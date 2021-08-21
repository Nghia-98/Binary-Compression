#include <stdio.h>
#include <conio.h>
#include <string>
#include <string.h>
#include <fstream>
#include <bitset>
#include <windows.h>
#include <vector>
#include <iostream>
#include <tchar.h>
#include <iomanip>
using namespace std;

#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable:4996)

struct NODE {
	unsigned char	c;		// ky tu
	int		freq;	// so lan xuat hien
	bool	used;	// danh dau node da xu ly chua 
	int		nLeft;	// chi so nut con nam ben trai
	int		nRight;	// chi so nut con nam ben phai
};

struct MABIT {
	char*	bits;
	int		soBit;
};

const int MAX_NODE = 256 * 9;
const int MAX_BIT_LEN = 10000;
NODE	huffTree[MAX_NODE];
MABIT	bangMaBit[256];

void KhoiTao() {
	for (int i = 0; i < MAX_NODE; i++) {
		huffTree[i].c = i;
		huffTree[i].freq = 0;
		huffTree[i].used = false;
		huffTree[i].nLeft = -1;
		huffTree[i].nRight = -1;
	}
}

void ThongKeTanSoXuatHien(string tenFile) {
	fstream fi(tenFile, ios::in | ios::binary);
	unsigned char c;

	while (1) {
		fi >> noskipws >> c;
		if (fi.eof()) {
			break;
		}
		huffTree[c].freq++; // Ghi chú: Thao tác này để làm gì (nghĩa là gì)?
	}
	fi.close();
}

bool Tim2PhanTuMin(int &i, int &j, int nNode) {
	i = -1;
	j = -1;

	// tim 2 phan tu co trong so nho nhat
	for (int k = 0; k < nNode; k++)
	if (huffTree[k].used == false && huffTree[k].freq > 0) { // Ghi chú: Tại sao cần kiểm tra 2 điều kiện này
		if (i == -1) {
			i = k;
		}
		else if (j == -1) {
			j = k;
		}
		else {
			if (huffTree[i].freq > huffTree[j].freq) {
				if (huffTree[k].freq < huffTree[i].freq) {
					i = k;
				}
			}
			else {
				if (huffTree[k].freq < huffTree[j].freq) {
					j = k;
				}
			}
		}
	}
	// sap xep lai 2 phan tu de co i: phan tu co trong so nho nhat, j: phan tu co trong so nho nhi	
	// co 2 truong hop can doi i,j:
	// huffTree[i].freq > huffTree[j].freq
	// huffTree[i].freq == huffTree[j].freq va (huffTree[i].c > huffTree[j].c)
	if (i != -1 && j != -1) {
		if ((huffTree[i].freq > huffTree[j].freq) || ((huffTree[i].freq == huffTree[j].freq) && (huffTree[i].c > huffTree[j].c))) {
			int t = i;
			i = j;
			j = t;
		}
		return true;
	}
	else {
		return false;
	}
}

int TaoCayHuffman() {
	int nNode = 256;
	int i, j;
	bool timThay = false;
	while (true) {
		// Ghi chú: bước này để làm gì
		timThay = Tim2PhanTuMin(i, j, nNode);
		if (!timThay) {
			break;
		}

		// Ghi chú: bước này để làm gì
		huffTree[nNode].c = (huffTree[i].c < huffTree[j].c) ? huffTree[i].c : huffTree[j].c;
		huffTree[nNode].freq = huffTree[i].freq + huffTree[j].freq;
		huffTree[nNode].nLeft = i;
		huffTree[nNode].nRight = j;

		// Ghi chú: bước này để làm gì
		huffTree[i].used = true;
		huffTree[j].used = true;

		// Ghi chú: bước này để làm gì
		huffTree[nNode].used = false;
		nNode++;

	}
	return nNode - 1; // Ghi chú: ý nghĩa của giá trị trả về?

}

void DuyetCayHuffman(int node, char maBit[], int nMaBit) {
	if (node == -1) {
		return;
	}
	if (huffTree[node].nLeft == -1 && huffTree[node].nRight == -1) {	//Ghi chú: ý nghĩa của điều kiện kiểm tra là gì?	
		bangMaBit[node].soBit = nMaBit;
		bangMaBit[node].bits = new char[nMaBit];
		for (int i = 0; i < nMaBit; i++) {
			bangMaBit[node].bits[i] = maBit[i];
		}
		return;
	}
	else {
		//Ghi chú: ý nghĩa của 2 dòng lệnh bên dưới là gì?
		maBit[nMaBit] = 0;
		DuyetCayHuffman(huffTree[node].nLeft, maBit, nMaBit + 1);

		//Ghi chú: ý nghĩa của 2 dòng lệnh bên dưới là gì?
		maBit[nMaBit] = 1;
		DuyetCayHuffman(huffTree[node].nRight, maBit, nMaBit + 1);
	}
}

void PhatSinhMaBit(int nRoot) { // Ghi chú: ý nghĩa của tham số nRoot?
	for (int i = 0; i < 256; i++) {
		bangMaBit[i].soBit = 0;
		bangMaBit[i].bits = NULL;
	};
	char maBit[MAX_BIT_LEN / 8];
	int nMaBit = 0;

	DuyetCayHuffman(nRoot, maBit, nMaBit);
}

void MaHoa1KyTu(unsigned char c, unsigned char &out, unsigned char &viTriBit, string &DuLieuNen) { 

	for (int i = 0; i < bangMaBit[c].soBit; i++) {
		if (bangMaBit[c].bits[i] == 1) {
			out = out | (1 << viTriBit); 
		}
		if (viTriBit == 0) { // da du 1 byte, ghi byte do ra file ( vi tri bit tu 7 xuống 0 là đủ 8 bit)
			viTriBit = 7;
			DuLieuNen += out; //Tong hop tat ca cac byte
			out = 0; //
		}
		else {
			viTriBit--;
		}
	}
}

void string_to_bit(string data, string &bit)
{
	for (int i = 0; i < data.length(); i++)
	{
		bitset<8> binary(data[i]);
		bit += binary.to_string();
	}
}

unsigned char PhatSinhKiTu(int curr, string &bitEnCoding)
{
	if (huffTree[curr].nLeft == -1 && huffTree[curr].nRight == -1) return huffTree[curr].c; // nếu đã đi đến node lá thì xuất ra kí tự tương ứng
	if (bitEnCoding[0] == '0') // gặp 0 thì đi qua trái
	{
		bitEnCoding.erase(0, 1);
		PhatSinhKiTu(huffTree[curr].nLeft, bitEnCoding);
	}
	else // gặp 1 thì đi qua phải
	{
		bitEnCoding.erase(0, 1);
		PhatSinhKiTu(huffTree[curr].nRight, bitEnCoding);
	}
}

string wchar_t2string(const wchar_t *wchar)
{
	string str = "";
	int index = 0;
	while (wchar[index] != 0)
	{
		str += (char)wchar[index];
		++index;
	}
	str[index] = NULL;
	return str;
}

wchar_t *string2wchar_t(const string &str)
{
	wchar_t *wchar = new wchar_t[str.size()];
	int index = 0;
	while (index < str.size())
	{
		wchar[index] = (wchar_t)str[index];
		++index;
	}
	wchar[str.size()] = '\0';
	return wchar;
}

vector<string> listFilesInDirectory(string directoryName)
{
	WIN32_FIND_DATA FindFileData;
	wchar_t * FileName = string2wchar_t(directoryName);
	HANDLE hFind = FindFirstFile(FileName, &FindFileData);

	vector<string> listFileNames;
	listFileNames.push_back(wchar_t2string(FindFileData.cFileName));

	while (FindNextFile(hFind, &FindFileData))
		listFileNames.push_back(wchar_t2string(FindFileData.cFileName));

	return listFileNames;
}

int listFile(string Folder_input, vector<string> &listNameFiles, vector<string> &listAddressFile)
{
	//cout << "\n Nhap vao duong dan foder : ";  // vd: "D:\\1612426-HW-Tuan11\\"
	//getline(cin, Folder_input);
	string temp1 = "*.*";
	string temp2 = Folder_input + temp1;
	listNameFiles = listFilesInDirectory(temp2);

	for (int i = 2; i < listNameFiles.size(); i++) // chay het cac file trong foder
	{
		string temp3 = Folder_input + listNameFiles[i]; // ghép đường dẫn folder với tên file để tạo thành đường dẫn file.
		listAddressFile.push_back(temp3);
	}
	for (int i = 0; i < listAddressFile.size(); i++) // xuất ra
		cout <<endl<< i + 1 << " : " << listNameFiles[i+2] << endl;

	return listAddressFile.size(); // trả về số file có trong folder
}

//BOOL WINAPI SetCurrentDirectory(_In_ LPCTSTR lpPathNam); //Cố định địa chỉ thao tác tại folder
//
//BOOL WINAPI CreateDirectory( //Khởi tạo một folder mới
//_In_  LPCTSTR     lpPathName,
//_In_opt_ LPSECURITY_ATTRIBUTES lpSecurityAttributes
//);

struct fileNen{                            // Cấu trúc lưu trữ dữ liệu của 1 file nén
    unsigned char doDaiTenFile;
	string tenFile;             
	int sizeTruocNen;                      // dung lượng file trước nén
	int sizeSauNen;                        // dung lượng file sau khi nén
	int bitBatDau;                         // vị trí bắt đầu đọc bitEnCoding để giải nén
	unsigned char soBitCuoiCoNghia;        // số bits có nghĩa trong bytes cuối 
	string DataNen_file;                   // chuỗi string bits dùng để giải nén
};

 void Nen_Folder(string folder_in, string file_out) // Nén tất cả các file trong folder_in và lưu kết quả nén vào tập tin file_out.
{
	vector<string> Ten_File; // danh sách tên file
	vector<string> Link_File; // danh sách đường dẫn file
	int soFile = listFile(folder_in, Ten_File, Link_File); // Tạo danh sách tên file và danh sách đường dẫn file có trong Folder

	// BUOC 1: thong ke tan suat xuat hien cua cac ki tu
	KhoiTao();
	for (int i = 0; i < soFile; i++) //duyệt hết các File để thống kê tần số xuất hiện của các kí tự
	{
		ThongKeTanSoXuatHien(Link_File[i]); 
	}
	// BUOC 2: tao cay Huffman
	int nRoot = TaoCayHuffman(); 
	// BUOC 3: phat sinh ma bit
	PhatSinhMaBit(nRoot);

	string DuLieuNen_Folder = "";
	fileNen *file = new fileNen[soFile];

	for (int i = 0; i < soFile; i++)
	{
		// BUOC 4: thay the cac ky tu bang ma bit tuong ung
		unsigned char out = 0;				// ky tu se xuat ra
		unsigned char soBitCoNghia = 0;		// byte cuoi co the ko su dung het cac bit nen can luu so bit co nghia cua byte cuoi

		unsigned char c;

		unsigned char viTriBit = 7;			//Ghi chú: ý nghĩa của biến viTriBit?

		string DuLieuNen_File = "";
		int sizefile = 0;

		// Duyệt file lần 2 thay kí tự bằng mã bit tương ứng
		fstream fi(Link_File[i], ios::in | ios::binary);
		while (true) {
			fi >> noskipws >> c;
			if (fi.eof()) {
				break;
			}
			sizefile++;
			MaHoa1KyTu(c, out, viTriBit, DuLieuNen_File);
		}

		soBitCoNghia = 7 - viTriBit;
		if (soBitCoNghia == 0) {
			soBitCoNghia = 8;
		}
		else {
			DuLieuNen_File += out;
		}

		// Tạo dữ liệu cho 1 struct fileNen để lát nữa ghi vào file nén
		file[i].doDaiTenFile = Ten_File[i + 2].length(); // độ dài của file nén
		file[i].tenFile = Ten_File[i + 2];               // tên của file được nén
		file[i].sizeTruocNen = sizefile;                 // size của file trước khi nén
		file[i].sizeSauNen = DuLieuNen_File.length();    // size của file sau khi nén ( độ dài của dataNen )
		file[i].soBitCuoiCoNghia = soBitCoNghia;         // số bit dùng để giải nén của bytes cuối
		file[i].bitBatDau = DuLieuNen_Folder.length() * 8; // Bit bắt đầu dùng để giải nén

		DuLieuNen_Folder += DuLieuNen_File;              // Cộng dồn data nén của từng file vào data nén của folder
		fi.close();
	}

	/* Ghi tất cả dữ liệu cần thiết ra file nén */
	/* Đầu tiên là các dữ liệu của header file */
	string maNhanDangFile = "HoangNghia"; // 10bytes
	//int maNhanDangFile = 1612426;
	
	fstream fout(file_out, ios::out | ios::binary); // mở file kiểu nhị phân để ghi
  
	for (int i = 0; i < maNhanDangFile.length(); i++) // ghi mã nhân dạng vào 10 bytes đầu của file
	{
		fout.write((const char*)&maNhanDangFile[i], sizeof(char));
	}
	for (int i = 0; i < 256; i++) // lưu lại bảng tần số xuất hiện của cái kí tự (dùng để tạo cây khi giải nén)
	{
		fout.write((const char*)& huffTree[i].freq, sizeof(huffTree[i].freq)); // 1 bytes (kiểu int)
	}

	fout.write((const char*)&soFile, sizeof(int)); // số file có trong folder

	for (int i = 0; i < soFile; i++) // ghi dữ liệu của 1 file nén
	{
		fout.write((const char *)& file[i].doDaiTenFile, sizeof(unsigned char)); // ghi độ dài tên file vào file nén
		fout << file[i].tenFile;                                                 // ghi tên file được nén ra file nén
		fout.write((const char *)& file[i].sizeTruocNen, sizeof(int));           // dung lượng file trước khi nén
		fout.write((const char *)& file[i].sizeSauNen, sizeof(int));             // dung lượng file sau khi nén
		fout.write((const char *)& file[i].soBitCuoiCoNghia, sizeof(unsigned char)); // số bits có nghĩa trong bytes cuối của dataNen
		fout.write((const char *)& file[i].bitBatDau, sizeof(int));              // vị trí bit bắt đầu đọc để giải nén
	}

	fout << DuLieuNen_Folder; // Ghi dữ liệu nén của tất cả các file trong folder

	fout.close();

	cout << "\n Nen thanh cong !\n";
} 

bool GiaiNen_Folder(string file_out, string folder_out) // giải nén tất cả các file trong tập tin file_out và lưu vào thư mục folder_out
{
	fstream fi(file_out,ios::in | ios::binary); // mở tập tin chứa dữ liệu nén để đọc (load data)
	string maNhanDangFile = "" ;              // mã nhận dạng file
	char a;
	
	for (int i = 0; i < 10; i++) // Đọc vào mã nhận dạng file 
	{
		fi.read((char*)& a, sizeof(char)); // đọc 10 bytes đầu của file vào biến 
		maNhanDangFile += a;
	}
	
	if (maNhanDangFile != "HoangNghia"){ // Nếu file có mã nhận dạng khác với file nén của chương trình này thì sẽ không thể giải nén được
		cout << endl << maNhanDangFile << " ==> Ma nhan dang file khong trung khop !!!";
		cout << "\n Khong nhan dang duoc File !!!!!!!\n";
		return false; // nếu mã nhận dạng file != "HoangNghia" thì dừng lại không giải nén nữa
	}

	/*Từ bảng tần suất lưu trong header file, ta sẽ tạo lại cây huffman và bản mã bit*/
	KhoiTao();
	int tanSo;
	for (int i = 0; i < 256; i++)  // lấy bản tần số từ file 
	{
		fi.read((char*)& tanSo, sizeof(int));
		huffTree[i].freq = tanSo;
	}
	int nRoot = TaoCayHuffman(); // tạo cây huffman, lưu lại vị trí node gốc
	PhatSinhMaBit(nRoot);        // tạo bản mã bit

	/*Lấy dữ liệu cấu trúc của từng file đã được nén*/
	int soLuongFile;
	fi.read((char*)& soLuongFile, sizeof(int));            // đọc vào số lượng file cần sử lí
	fileNen *file = new fileNen[soLuongFile];              // mảng các cấu trúc file nén
	int *DeCoding_File_Size = new int[soLuongFile];        // lưu lại dung lượng file giải nén (phục vụ cho việc check sum khi giải nén)
	for (int i = 0; i < soLuongFile; i++)
	{
		file[i].tenFile = "";  // ban đầu khởi tạo tenFile là chuỗi rỗng
		fi.read((char*)& file[i].doDaiTenFile, sizeof(unsigned char));     // đọc vào độ dài tên file
		for (int j = 0; j < file[i].doDaiTenFile; j++)                     // đọc vào tên file
		{
			char c;
			fi.read((char*)& c, 1);
			file[i].tenFile += c;
		}
		fi.read((char*)& file[i].sizeTruocNen, sizeof(int));
		fi.read((char*)& file[i].sizeSauNen, sizeof(int));
		fi.read((char*)& file[i].soBitCuoiCoNghia, sizeof(unsigned char));
		fi.read((char*)& file[i].bitBatDau, sizeof(int));
	}
	/* Lúc này ta đã đọc đến phần Data nén của các file */
	string bit_EnCoding = ""; // mảng string bits dùng để giải nén
	unsigned char c;
	while (fi >> noskipws >> c)
	{
		bitset<8> binary(c);
		bit_EnCoding += binary.to_string();
	}
	fi.close();
	/* Tách riêng chuỗi string bits code ( data nén ) cho từng file */
	for (int i = 0; i < soLuongFile; i++) // 
	{
		file[i].DataNen_file = bit_EnCoding.substr(file[i].bitBatDau, file[i].sizeSauNen * 8); // lấy ra đoạn string bits code của mỗi file

		int soBitKhongCoNghia = 8 - file[i].soBitCuoiCoNghia;
		if (soBitKhongCoNghia != 0)
			file[i].DataNen_file.erase(file[i].DataNen_file.size() - soBitKhongCoNghia, soBitKhongCoNghia); // nếu tồn tại những bit ko có nghĩa thì xóa đi
	}
	/* Khi có dữ liệu nén của từng file ta bắt đầu giải nén từng file một */

	int n = folder_out.length();
	TCHAR *folder = new TCHAR[n + 1];
	for (int i = 0; i < n; i++)
		folder[i] = folder_out[i];
	folder[n] = NULL;

	CreateDirectory(folder, NULL); // mở thư mục để làm việc
	SetCurrentDirectory(folder);   // cố định chỉ thao tác ở folder này
	for (int i = 0; i < soLuongFile; i++)
	{
		if (i == 0) cout << "\n Dang giai nen file thu nhat. Vui long doi :)";
		if (i != 0) cout << "\n Dang giai nen file thu " << i + 1 << ". Vui long doi :)";
		
		string DuLieuGiaiNen = "";
		while (file[i].DataNen_file.length()>0)
		{
			int tmp = nRoot;  // lưu lại node root của cây huffman (phục vụ cho lần gọi đệ quy ở vòng lặp tiếp theo)
			DuLieuGiaiNen += PhatSinhKiTu(tmp, file[i].DataNen_file); // lưu các kí tự được giải mã vào 1 chuỗi string
		}
		DeCoding_File_Size[i] = DuLieuGiaiNen.length();               // lưu lại dung lượng file giải nén
		/* Khi đã giải mã hết chuỗi file[i].dataNen_file thì ghi hết ra file */
		
		fstream fo(file[i].tenFile, ios::out | ios::binary); // mở file
		fo << DuLieuGiaiNen;
		fo.close();
	}
	/* Xuất kết quả check sum */
	cout << endl;
	cout << endl << setw(30) << left << "Ten File"  << left << setw(22) << "Dung luong truoc nen" <<  left <<setw(29) << "Dung luong sau khi giai nen";
	for (int i = 0; i < soLuongFile; i++)
	{
		cout << endl << setw(30) << left << file[i].tenFile << left << setw(22) << file[i].sizeTruocNen << left << setw(29)<< DeCoding_File_Size[i];
		if (file[i].sizeTruocNen == DeCoding_File_Size[i]) cout << right << setw(27) << "Checksum complete with no error! \n";
		else cout << right << setw(27) << "Checksum complete with error! \n";
	}
	cout << "\n Giai nen thanh cong !\n";
	return true;
}

bool XemThongTinFileNen(string file_nen)
{
	fstream fi(file_nen, ios::in | ios::binary); // mở file nén để đọc thông tin
	string maNhanDangFile = "";              // mã nhận dạng file
	char a;

	for (int i = 0; i < 10; i++)
	{
		fi.read((char*)& a, sizeof(char)); // đọc 10 bytes đầu của file vào biến 
		maNhanDangFile += a;
	}

	if (maNhanDangFile != "HoangNghia"){
		cout << endl <<" Ma Nhan Dang File : " << maNhanDangFile;
		cout << "\n Khong nhan dang duoc File !!!!!! \n";
		return false; // nếu mã nhận dạng file != "HoangNghia" thì dừng lại không đọc tiếp nữa
	}

	/*Đọc qua bảng tần số*/
	int tanSo;
	for (int i = 0; i < 256; i++)  // lấy bản tần số từ file 
	{
		fi.read((char*)& tanSo, sizeof(int));
	}
	
	/*Lấy dữ liệu cấu trúc của từng file đã được nén*/
	int soLuongFile;
	fi.read((char*)& soLuongFile, sizeof(int));            // đọc vào số lượng file cần sử lí
	fileNen *file = new fileNen[soLuongFile];              // mảng các cấu trúc file nén
	int *DeCoding_File_Size = new int[soLuongFile];        // lưu lại dung lượng file giải nén (phục vụ cho việc check sum khi giải nén)
	for (int i = 0; i < soLuongFile; i++)
	{
		file[i].tenFile = "";  // ban đầu khởi tạo tenFile là chuỗi rỗng
		fi.read((char*)& file[i].doDaiTenFile, sizeof(unsigned char));     // đọc vào độ dài tên file
		for (int j = 0; j < file[i].doDaiTenFile; j++)                     // đọc vào tên file
		{
			char c;
			fi.read((char*)& c, 1);
			file[i].tenFile += c;
		}
		fi.read((char*)& file[i].sizeTruocNen, sizeof(int));               // đọc vào Size trước nén
		fi.read((char*)& file[i].sizeSauNen, sizeof(int));                 // đọc vào Size sau nén
		fi.read((char*)& file[i].soBitCuoiCoNghia, sizeof(unsigned char)); // đọc vào số bit có nghĩa
		fi.read((char*)& file[i].bitBatDau, sizeof(int));                  // đọc vào bit bắt đầu
	}
	
	/* Sau khi đọc đủ thông tin cần thiết, ta sẽ xuất thông tin từng file ra màn hình */
	cout << endl << setw(10) << left << "STT " << left << setw(35) << "Ten File"      << left << setw(25) << "Size truoc nen"     << left << "Size sau nen\n";
	for (int i = 0; i < soLuongFile; i++)
	{
		cout << setw(10) <<  left   <<   i+1 << left << setw(35) << file[i].tenFile << left << setw(25) << file[i].sizeTruocNen << left << file[i].sizeSauNen << endl;
	}
	return true;
}

bool GiaiNen_TheoFile(string file_nen, string folder_out)
{
	fstream fi(file_nen, ios::in | ios::binary); // mở tập tin chứa dữ liệu nén để đọc (load data)
	string maNhanDangFile = "";              // mã nhận dạng file
	char a;
	for (int i = 0; i < 10; i++)
	{
		fi.read((char*)& a, sizeof(char)); // đọc 10 bytes đầu của file vào biến (mã nhận dạng file "HoangNghia")
		maNhanDangFile += a;
	}

	if (maNhanDangFile != "HoangNghia"){
		cout << maNhanDangFile;
		cout << "\n Khong nhan dang duoc File !!!!!!!\n";
		return false; // nếu mã nhận dạng file != "HoangNghia" thì dừng lại không giải nén nữa
	}

	/*Từ bảng tần suất lưu trong header file, ta sẽ tạo lại cây huffman và bản mã bit*/
	KhoiTao();
	int tanSo;
	for (int i = 0; i < 256; i++)  // lấy bản tần số từ file 
	{
		fi.read((char*)& tanSo, sizeof(int));
		huffTree[i].freq = tanSo;
	}
	int nRoot = TaoCayHuffman(); // tạo cây huffman, lưu lại vị trí node gốc
	PhatSinhMaBit(nRoot);        // tạo bản mã bit

	/*Lấy dữ liệu cấu trúc của từng file đã được nén*/
	int soLuongFile;
	fi.read((char*)& soLuongFile, sizeof(int));            // đọc vào số lượng file cần sử lí
	fileNen *file = new fileNen[soLuongFile];              // mảng các cấu trúc file nén (lưu trữ thông tin của 1 file con đã nén trong 1 folder)
	int *DeCoding_File_Size = new int[soLuongFile];        // lưu lại dung lượng file giải nén (phục vụ cho việc check sum khi giải nén)
	for (int i = 0; i < soLuongFile; i++)
	{
		file[i].tenFile = "";  // ban đầu khởi tạo tenFile là chuỗi rỗng
		fi.read((char*)& file[i].doDaiTenFile, sizeof(unsigned char));     // đọc vào độ dài tên file
		for (int j = 0; j < file[i].doDaiTenFile; j++)                     // đọc vào tên file
		{
			char c;
			fi.read((char*)& c, 1);
			file[i].tenFile += c;
		}
		fi.read((char*)& file[i].sizeTruocNen, sizeof(int));               // dung lượng file trước khi nén
		fi.read((char*)& file[i].sizeSauNen, sizeof(int));                 // dung lượng dile sau khi nén
		fi.read((char*)& file[i].soBitCuoiCoNghia, sizeof(unsigned char)); // số bits của bytes cuối có nghĩa
		fi.read((char*)& file[i].bitBatDau, sizeof(int));                  // vị trí bits đầu tiên
	}
	/* Lúc này ta đã đọc đến phần Data nén của các file */
	string bit_EnCoding = ""; // mảng string bits dùng để giải nén
	unsigned char c;
	while (fi >> noskipws >> c)
	{
		bitset<8> binary(c);
		bit_EnCoding += binary.to_string();
	}
	fi.close(); // đọc hết dữ liệu từ file nén thif đóng file nén lại
	/* Tách riêng chuỗi string bits code ( data nén ) cho từng file */
	for (int i = 0; i < soLuongFile; i++) // 
	{
		file[i].DataNen_file = bit_EnCoding.substr(file[i].bitBatDau, file[i].sizeSauNen * 8); // lấy ra đoạn string bits code của mỗi file

		int soBitKhongCoNghia = 8 - file[i].soBitCuoiCoNghia;
		if (soBitKhongCoNghia != 0)
			file[i].DataNen_file.erase(file[i].DataNen_file.size() - soBitKhongCoNghia, soBitKhongCoNghia); // nếu tồn tại những bit ko có nghĩa thì xóa đi
	}
	/* Khi có dữ liệu nén của từng file ta bắt đầu giải nén các file được chọn */
	/* Xuat danh sách file cho người dùng dễ chọn */
	cout << endl << setw(10) << left << "STT " << left << setw(35) << "Ten File" << left << setw(25) << "Size truoc nen" << left << "Size sau nen\n";
	for (int i = 0; i < soLuongFile; i++)
	{
		cout << setw(10) << left << i + 1 << left << setw(35) << file[i].tenFile << left << setw(25) << file[i].sizeTruocNen << left << file[i].sizeSauNen << endl;
	}

	int n = folder_out.length();
	TCHAR *folder = new TCHAR[n + 1];
	for (int i = 0; i < n; i++)
		folder[i] = folder_out[i];
	folder[n] = NULL;

	CreateDirectory(folder, NULL); // mở thư mục để làm việc
	SetCurrentDirectory(folder);   // cố định chỉ thao tác ở folder này

	vector<int> File;
	
	char tieptuc = 'y';
	cout << "\n Moi nhap thu tu cac file can giai nen : ";
	for (int i = 0; i < soLuongFile; i++)
	{
		int a;
		cout << "\ngiai nen file : ";
		cin >> a;
		if (a < 1 || a > soLuongFile){
			cout << "\n Thu tu file khong duoc be hon 1 hoac lon hon so luong file co trong folder ! Moi nhap lai .\n";
		}
		else
			File.push_back(a);

		cout << " Ban co muon nhap tiep (y/n) : ";
		cin >> tieptuc;
		if (tieptuc != 'y') break;
	}

	cout << endl << "Thu tu cac file dc chon de giai nen la : ";
	for (int i = 0; i < File.size(); i ++ )
	{
		cout << File[i] << "  ";
	}

	cout << "\n Dang trong qua trinh giai nen! ....\n";
	for (int i = 0; i < File.size(); i++)
	{
		if (File[i] == 1) cout << "\n Dang giai nen file thu nhat. Vui long doi :)";
		else             cout << "\n Dang giai nen file thu " << File[i] << ". Vui long doi :)";

		string DuLieuGiaiNen = "";
		while (file[File[i] - 1].DataNen_file.length()>0) //nếu file được chọn giải nén là file thứ m => đó là file nằm ở vị trí thứ m-1 trong mảng file (xem lại dòng 576)
		{
			int tmp = nRoot;  // lưu lại node root của cây huffman (phục vụ cho lần gọi đệ quy ở vòng lặp tiếp theo)
			DuLieuGiaiNen += PhatSinhKiTu(tmp, file[File[i]-1].DataNen_file); // lưu các kí tự được giải mã vào 1 chuỗi string
		}
		DeCoding_File_Size[File[i]-1] = DuLieuGiaiNen.length();               // lưu lại dung lượng file giải nén
		/* Khi đã giải mã hết chuỗi file[i].dataNen_file thì ghi hết ra file */

		fstream fo(file[File[i]-1].tenFile, ios::out | ios::binary); // mở file
		fo << DuLieuGiaiNen;
		fo.close();
	}
	/* Xuất kết quả check sum */
	cout << endl;
	cout << endl << setw(30) << left << "Ten File" << left << setw(22) << "Dung luong truoc nen" << left << setw(29) << "Dung luong sau khi giai nen";
	for (int i = 0; i < File.size(); i++)
	{
		cout << endl << setw(30) << left << file[File[i]-1].tenFile << left << setw(22) << file[File[i]-1].sizeTruocNen << left << setw(29) << DeCoding_File_Size[File[i]-1];
		if (file[File[i]-1].sizeTruocNen == DeCoding_File_Size[File[i]-1]) cout << right << setw(27) << "Checksum complete with no error! \n";
		else cout << right << setw(27) << "Checksum complete with error! \n";
	}
	cout << "\n Giai nen thanh cong !\n";
	return true;
}

