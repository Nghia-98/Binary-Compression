#include<iostream>
using namespace std;

void Menu(int &choice)
{
	system("cls");
	cout << "\n    *=*=*=*=*==*=*=*=*=*=*=*=*=*=*=*=*=*=*=* Huffman Compression *=*=*=*=*=*=*==*=*=*=*=*=*=*=*=*=*=*=*=* \n";
	cout <<   "    *=*                               1612426 - Hoang Nghia - Project_2                               *=*  ";
	cout << "\n    *=*=*=*=*==*=*=*=*=*=*=*=*=*=*=*=*=*=*=* Huffman Compression *=*=*=*=*=*=*==*=*=*=*=*=*=*=*=*=*=*=*=* \n";
	cout << " Menu:" << endl;
	cout << " 1.  Nen cac file trong folder." << endl;
	cout << " 2.  Xem noi dung file da nen." << endl;
	cout << " 3.  Giai nen tat ca cac file trong tap tin nen." << endl;
	cout << " 4.  Chon va giai nen cac file rieng le." << endl;
	cout << " 5.  Thoat!" << endl;
	cout << " --------------------------------------------------";
	do{
		cout << "\n  Lua chon cua ban : ";
		cin >> choice;
		if (choice < 1 || choice > 5){
			cout << "Lua chon cua ban khong hop le ! ...Vui long chon lai.\n";
		}
	} while (choice < 1 || choice > 5);
}