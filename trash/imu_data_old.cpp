



__u8 reg = 0x00;

int response = i2c_smbus_read_byte_data(file, reg);
  if (response < 0) {
    /* ERROR HANDLING: i2c transaction failed */
  } else {
    /* res contains the read word */
  }
cout << "Response: 0x" << hex << response << '\n';