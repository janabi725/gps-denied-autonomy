import rclpy
from rclpy.node import Node
from sensor_msgs.msg import Image
from cv_bridge import CvBridge
import cv2
import numpy as np

class MiniTheseusOdometry(Node):
    def __init__(self):
        super().__init__('vio_odometry')
        self.publisher_ = self.create_publisher(Image, '/image_raw', 10)
        self.bridge = CvBridge()
        
        self.cap = cv2.VideoCapture('/dev/video10')
        if not self.cap.isOpened():
            self.get_logger().error("Konnte /dev/video10 nicht öffnen!")
            
        # VIO Parameter: Lucas-Kanade Optical Flow
        self.lk_params = dict(winSize=(15, 15), maxLevel=2,
                              criteria=(cv2.TERM_CRITERIA_EPS | cv2.TERM_CRITERIA_COUNT, 10, 0.03))
        
        # VIO Parameter: Feature Detection (Deine grünen Punkte)
        self.feature_params = dict(maxCorners=100, qualityLevel=0.03, minDistance=7, blockSize=7)
        
        # Speicher für das vorherige Bild und die vorherigen Punkte
        self.old_gray = None
        self.p0 = None
        
        self.timer = self.create_timer(0.033, self.timer_callback)
        self.get_logger().info('Odometrie gestartet. Bewege die Kamera langsam!')

    def timer_callback(self):
        ret, frame = self.cap.read()
        if not ret or frame is None:
            return
            
        frame_gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
        
        # 1. Initiale Feature-Suche (oder wenn wir Punkte verloren haben)
        if self.old_gray is None or self.p0 is None or len(self.p0) < 15:
            self.p0 = cv2.goodFeaturesToTrack(frame_gray, **self.feature_params)
            self.old_gray = frame_gray.copy()
            self.get_logger().info('Neue Referenzpunkte gesucht...', throttle_duration_sec=2.0)
            return
            
        # 2. Optical Flow: Wo sind die Punkte im neuen Bild hin?
        p1, status, err = cv2.calcOpticalFlowPyrLK(self.old_gray, frame_gray, self.p0, None, **self.lk_params)
        
        # 3. Nur die Punkte behalten, die erfolgreich verfolgt wurden (status == 1)
        if p1 is not None:
            good_new = p1[status == 1]
            good_old = self.p0[status == 1]
            
            # 4. Mathematik: Wie weit haben sich die Punkte im Durchschnitt bewegt?
            if len(good_new) > 0:
                dx = np.mean(good_new[:, 0] - good_old[:, 0]) # Bewegung auf der X-Achse
                dy = np.mean(good_new[:, 1] - good_old[:, 1]) # Bewegung auf der Y-Achse
                
                # Nur loggen, wenn die Bewegung signifikant ist (kein Rauschen)
                if abs(dx) > 1.5 or abs(dy) > 1.5:
                    direction_x = "RECHTS" if dx < 0 else "LINKS"
                    direction_y = "UNTEN" if dy < 0 else "OBEN"
                    self.get_logger().info(f'Kamera bewegt sich -> {direction_x} | {direction_y} (dx: {dx:.1f}, dy: {dy:.1f})')
                    
            # 5. Aktualisiere die Variablen für den nächsten Durchlauf (Das "neue" Bild wird jetzt zum "alten" Bild)
            self.old_gray = frame_gray.copy()
            self.p0 = good_new.reshape(-1, 1, 2)
        else:
            self.p0 = None # Reset, falls alle Punkte verloren gingen
            
        # Bild im Hintergrund für ROS publizieren
        msg = self.bridge.cv2_to_imgmsg(frame_gray, encoding="mono8")
        self.publisher_.publish(msg)

    def destroy_node(self):
        self.cap.release()
        super().destroy_node()

def main(args=None):
    rclpy.init(args=args)
    node = MiniTheseusOdometry()
    try:
        rclpy.spin(node)
    except KeyboardInterrupt:
        pass
    finally:
        node.destroy_node()
        rclpy.shutdown()

if __name__ == '__main__':
    main()