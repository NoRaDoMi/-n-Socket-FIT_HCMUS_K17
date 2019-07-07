
<img src="https://lh3.googleusercontent.com/xMjC1Qzg1OuyB2EbdFfkTriUPInVCcVv41YOn4eW0ClrvIWXDvu0pLgh-jswmm6UoqCg" width="800" height="200" />

# ĐỒ ÁN CUỐI MẠNG MÁY TÍNH K17
## Thực hiện: 
      1712126 - Võ Trọng Phúc
      1712148 - Hà Duy Tân
      1712149 - Trịnh Tấn Tài
      1712106 - Nguyễn Phúc Khôi Nguyên
      
## Mô tả

      Một chương trình Proxy Server sử dụng cho HTTP có những chức năng sau:
      1.	Hỗ trợ HTTP 1.0 và HTTP 1.1
      2.	Chương trình cho phép Client truy cập website thông qua Proxy Server (GET, POST)
      3.	Chương trình chỉ  hỗ trợ http, không hỗ trợ https.
      4.	Proxy Server phải xử lý đồng thời được các request từ client.
      5.	Proxy Server sẽ chạy trên port 8888.
      6.	Proxy Server cấu hình 1 file blacklist.conf, mỗi dòng chứa các domain website cấm Client truy cập. Proxy Server sẽ chặn tất cả các truy cập trùng với các domain trong file.
      Ví dụ:
      abc.com.vn www.thongtin.net
      ...
      7.	Proxy Server có chức năng caching lại những nội dung của trang web, để phục vụ cho Client khác nhanh hơn. 
      
![img1](https://live.staticflickr.com/65535/48218800342_6fe2e0f81b_b.jpg)
 ## Các thức test chương trình
    1.	Sử dụng web browser: dùng Firefox hoặc Google Chrome (cả 2 được cấu hình proxy đến Proxy Server, port 8888), truy cập các website và hiển thị được nội dung của trang web.
    2.	Khi Client truy cập các trang bị cấm, Client nhận được kết quả 403 (Forbidden) HTTP response. The Proxy Server không được gửi bất kỳ request nào tới web server.
## Demo
<img src="https://live.staticflickr.com/65535/48218798116_457858f394_b.jpg" width="700" height="400" />
<br/>
<img src="https://live.staticflickr.com/65535/48218798521_c11994f1c0_o.png" width="700" height="400" />
<br/>
<img src="https://live.staticflickr.com/65535/48218798416_689e5e7a98_o.png" width="700" height="400" />
<img src="https://live.staticflickr.com/65535/48218846842_c3e62e2f02_o.png" width="700" height="400" />
