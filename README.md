# README - Node LoRa

## 1. Giới Thiệu
Node LoRa là thiết bị IoT sử dụng giao tiếp LoRa để truyền và nhận dữ liệu với Gateway. Thiết bị có thể hoạt động ở nhiều chế độ khác nhau (cảm biến hoặc relay) và gửi dữ liệu định kỳ đến Gateway.

## 2. Cấu Hình Thiết Bị
### 2.1. Địa Chỉ Thiết Bị (Dev Address)
```cpp
#define DEV_ID 0xAABBCCDDEE03
```
Mỗi thiết bị có một địa chỉ duy nhất (DEV_ID) dùng để định danh trong hệ thống.

### 2.2. Loại Thiết Bị (Dev Type)
```cpp
#define SENSOR_DEV_TYPE 0
#define RELAY_DEV_TYPE 1

#define DEV_TYPE SENSOR_DEV_TYPE
```
Thiết bị có thể hoạt động ở hai chế độ:
- **SENSOR_DEV_TYPE (0)**: Thiết bị hoạt động như một cảm biến, gửi dữ liệu đo lường lên Gateway.
- **RELAY_DEV_TYPE (1)**: Thiết bị hoạt động như một relay, có thể nhận lệnh từ Gateway để bật/tắt thiết bị khác.

### 2.3. Tần Suất Gửi Dữ Liệu
```cpp
#define REPORT_INTERVAL_MS 5000    // ms. 0 <-> No report
```
- Xác định khoảng thời gian (tính bằng mili-giây) để Node gửi dữ liệu cảm biến lên Gateway.
- Nếu giá trị là `0`, Node sẽ không tự động gửi dữ liệu mà chỉ gửi khi có sự kiện.

### 2.4. Cấu Hình Chân Kết Nối (Pin Config)
```cpp
#define LORA_TX_PIN 2
#define LORA_RX_PIN 3
```
- **LORA_TX_PIN (chân 2)**: Chân TX kết nối với module LoRa.
- **LORA_RX_PIN (chân 3)**: Chân RX kết nối với module LoRa.

## 3. Cách Hoạt Động
1. Node LoRa khởi động và đọc cấu hình thiết bị.
2. Nếu là cảm biến (`SENSOR_DEV_TYPE`):
   - Định kỳ (`REPORT_INTERVAL_MS`) gửi dữ liệu lên Gateway.
3. Nếu là relay (`RELAY_DEV_TYPE`):
   - Chờ lệnh điều khiển từ Gateway để bật/tắt thiết bị.
4. Dữ liệu được truyền qua giao thức LoRa đến Gateway để xử lý.

## 4. Ghi Chú
- Cần đảm bảo cấu hình đúng loại thiết bị (`DEV_TYPE`).
- Đảm bảo khoảng thời gian gửi dữ liệu phù hợp để tránh tắc nghẽn mạng LoRa.
- Kiểm tra kết nối giữa Node và Gateway để đảm bảo truyền dữ liệu ổn định.

