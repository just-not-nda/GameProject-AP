# **GOOGLE PACMAN**

Nguyễn Đức Anh - Mã SV: 24022763

## 1. Mô tả:
Là game đồ họa pixel sử dụng C++ và SDL2 lấy chủ đề từ tựa game [Google Pacman Doodle](https://www.google.com/logos/2010/pacman10-i.html) do Google ra mắt năm 2010 nhằm kỉ niệm 30 năm Pacman ra đời. 

## 2. Demo:
![Image](https://github.com/user-attachments/assets/fd9aeef3-4feb-4d8b-b252-eefb37ef379e)

![Image](https://github.com/user-attachments/assets/200913fb-a9bb-4a4f-8d07-b10459532e6c)

![Image](https://github.com/user-attachments/assets/f9dd4568-3c58-4707-8e93-26f5aaee5966)


## 3. Gameplay:
### a. Mục tiêu: 
Người chơi điều khiển nhân vật **Pac-Man** di chuyển trong mê cung, ăn hết các **chấm nhỏ (dot)** để qua level mới và cố gắng ghi điểm cao nhất. Tránh né các **con ma (ghost)** đang rượt đuổi bạn! Bạn có thể ăn các **Power Pellets (chấm to)** để tạm thời khiến các bóng ma yếu đi và có thể bị ăn ngược lại.

### b. Cách điều khiển:
- Sử dụng các phím mũi tên:
`↑ ↓ ← →` để di chuyển Pac-Man theo 4 hướng lên, xuống, trái, phải
- Nhấn phím `Esc` để tạm dừng game và thoát ra **Menu**

### c. Các loại Ghost:
- `Blinky` : màu đỏ, đuổi theo **Pacman**.
- `Pinky` : màu hồng, chặn đầu **Pacman** bằng cách đi đến vị trí trước **Pacman** 4 ô.
- `Inky` : màu xanh dương, đi đến vị trí đối xứng với vị trí của `Blinky` qua **Pacman**.
- `Clyde` : màu cam, đuổi theo **Pacman** nếu khoảng cách giữa nó và **Pacman** lớn hơn 8 ô, nếu không thì quay về vị trí mặc định của mình

### d. Tính năng đặc biệt:
- Bản đồ dạng `tile-based`:
Bản đồ được chia thành một lưới gồm nhiều **tile vuông (gạch)**. Mỗi tile mang giá trị khác nhau đại diện cho các cấu trúc của map như tường, dot, power pellect, v.v...

- Sử dụng thuật toán **đường đi ngắn nhất** `BFS` (tìm kiếm theo chiều rộng) để xử lí `Ghost` đuổi theo `Pacman`.

## 4. Công nghệ sử dụng
- C++
- SDL2
- App Piskel dùng để vẽ nhân vật, map, thành phần bằng pixel
