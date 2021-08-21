#include"resource.h"
#include"Menu.h"

int main()
{
	int choice;
cmd:Menu(choice);

	if (choice == 1) // nén tất cả các file trong folder_in và lưu kết quả vào tập tin file_out
	{
		string folder_in,file_out; // Yêu cầu user nhập vào folder chứa các file cần nén (folder_in) và nhập đường dẫn + tên file nén(file_out).
		cout << "\n Nhap vao folder ban muon nen (folder_in\\\\) : ";
		_flushall(); getline(cin, folder_in); // Nhập vào đường dẫn folder_in
		cout << "\n Nhap vao duong dan + ten file nen (file_out) : ";
		_flushall(); getline(cin, file_out);

		Nen_Folder(folder_in, file_out); // Gọi hàm nén folder ở tập tin resource.h

		system("pause");
		system("cls");
		goto cmd;
	}
	if (choice == 2) // xem thông tin file_out
	{
		string file_out;
		cout << "\n Nhap vao file nen can xem thong tin(file_out) : ";
		_flushall(); getline(cin, file_out);

		XemThongTinFileNen(file_out); // gọi hàm đọc và xuất thông tin file nén trong tep tin resource.h

		system("pause");
		system("cls");
		goto cmd;

	}
	if (choice == 3) // giải nén tất cả các file trong tập tin đã được nén (file_out), và lưu kết quả vào 1 thư mục (folder_out)
	{
		string file_out, folder_out;
		cout << "\n Nhap vao tap tin can giai nen(file_out) : ";
		_flushall(); getline(cin, file_out);
		cout << "\n Nhap vao folder chua cac file giai nen(folder_out\\\\) : ";
		_flushall(); getline(cin, folder_out);

		GiaiNen_Folder(file_out, folder_out);

		system("pause");
		system("cls");
		goto cmd;
	}
	if (choice == 4) // giải nén theo từng file riêng rẻ
	{
		string file_out, folder_out;
		cout << "\n Nhap vao file nen(file_out) : ";
		_flushall(); getline(cin, file_out);
		cout << "\n Nhap vao folder chua cac file nen : ";
		_flushall(); getline(cin, folder_out);

		GiaiNen_TheoFile(file_out, folder_out); // gọi hàm giải nén từng file riêng rẻ trong tệp tin resource.h

		system("pause");
		system("cls");
		goto cmd;
	}
	if (choice == 5)
	{
		return 0;
	}
	return 0;
}