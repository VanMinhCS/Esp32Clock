Một chiếc đồng hồ Decore với bộ điều khiển chính là Esp32 (module devkit V1), cho phép hiển thị cả lịch dương và lịch âm, hiển thị trên LCD 1602 I2C.  
Thiết bị sẽ kết nối Wifi (băng tần 2.4 GHz), truy cập vào server Google NTP để cập nhật thời gian sau đó chạy local (cập nhật thời gian qua NTP server mỗi 30 phút).  
Sau khi thiết bị hoạt động ổn định, bấm BOOT để chuyển đổi lịch dương và lịch âm.  
Dòng chữ khi xuất hiện có thể tùy chỉnh ở hàm setup() tùy sở thích ✍️(◔◡◔)  
Thư viện chuyển đổi lịch dương sang lịch âm: [Link](https://github.com/ESPACE-HCM/DateTimeConverter)  
Nền tảng: PlatformIO IDE.  
Cách nối dây giữa LCD 1602 i2c và Board ESP32: SDA - D21, SCL - D22, VCC - Vin, GND - GND.  
Tải, nạp code vào ESP32 và thưởng thức thôi UwU.  
