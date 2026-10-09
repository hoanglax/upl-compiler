# UPL Compiler (BTL01)

Bộ phân tích từ vựng, cú pháp và dựng AST cho ngôn ngữ UPL, viết bằng
Flex và Bison (C).

## Yêu cầu
gcc, make, flex, bison (Ubuntu/WSL: `sudo apt install gcc make flex bison`)

## Build và chạy
    make                              # build -> build/upl
    build/upl file.upl                # phân tích cú pháp, in AST nếu không có lỗi
    build/upl --tokens file.upl       # chỉ in danh sách token (kiểm tra lexer)
    build/upl --quiet file.upl        # không in AST
    make test                         # chạy bộ kiểm thử tự động
    make examples                     # chạy các ví dụ, ghi kết quả vào examples/output/
    make clean

Mã thoát: 0 = không lỗi, 1 = có lỗi, 2 = sai cách dùng hoặc không mở được file.

## Cấu trúc thư mục
| Thư mục | Nội dung |
|---|---|
| `docs/` | Văn phạm CFG (nộp bài), đặc tả ngôn ngữ |
| `src/lexer/upl.l` | Luật Flex: token, bỏ comment, lỗi từ vựng, theo dõi dòng/cột |
| `src/parser/upl.y` | Luật Bison: văn phạm, dựng AST, phục hồi lỗi |
| `src/ast/` | Định nghĩa node (`ast.h`), hàm tạo/giải phóng (`ast.c`), in cây (`ast_print.c`) |
| `src/common/` | Vị trí (`location.h`), báo lỗi tập trung (`error.c`), tên token |
| `src/semantic/` | Dành cho kiểm tra ngữ nghĩa (chưa làm) |
| `src/main.c` | Điểm vào chương trình |
| `tests/` | `valid/`, `lexical_errors/`, `syntax_errors/` và `run_tests.sh` |
| `examples/` | Chương trình UPL minh họa và `output/` (kết quả chạy) |
| `build/` | File sinh ra khi build |

## Các giả định về ngôn ngữ (đề bài chưa nêu rõ)
1. `for (khởi_tạo; điều_kiện; cập_nhật) { ... }` kiểu C; khởi tạo là khai
   báo hoặc gán, cập nhật là gán.
2. Có thêm hằng boolean `true`, `false`.
3. Thân `if`, `else`, `do`, `for` bắt buộc nằm trong `{ }`.
4. So sánh (`>`, `>=`, `==`) không kết hợp: `a > b > c` là lỗi cú pháp.
5. Chỉ kiểm tra cú pháp. Khai báo trước khi dùng, khai báo lại, tương
   thích kiểu thuộc giai đoạn ngữ nghĩa, chưa cài đặt.
6. Định danh: `[A-Za-z]+[0-9]*`. Hằng số: `[0-9]+`.

## Xử lý lỗi
- **Lỗi từ vựng**: ký tự lạ, định danh/số sai dạng (`1abc`, `a1b`, `_x`),
  số quá lớn, comment `/* */` không đóng. Lexer báo lỗi rồi đọc tiếp.
- **Lỗi cú pháp**: parser phục hồi bằng token `error` của Bison
  (đồng bộ ở `;` cho câu lệnh, ở `)` cho điều kiện `if`), nên một lần chạy
  báo được nhiều lỗi. Có lỗi thì không in AST.
- Định dạng: `file:dòng:cột: loại lỗi: mô tả`.

## Hạn chế đã biết
- Sau một lỗi cú pháp có thể phát sinh thêm lỗi dây chuyền.
- Khi có lỗi cú pháp, một số node AST dở dang không được giải phóng
  (chương trình thoát ngay sau đó).
- Chưa có kiểm tra ngữ nghĩa.