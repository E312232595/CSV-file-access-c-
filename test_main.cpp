#include <iostream>
#include "csv_access.hpp"

using namespace std;
/* run this program using the console pauser or add your own getch, system("pause") or input loop */

int main(int argc, char** argv) {
	
	class CSVFileAccess csv_file;
	char strbuff[100];
	int d;
	cout<<"#################csv access proj test######################"<<endl;
	
	//���ļ� 
	csv_file.Open("test.csv");
	//��ȡ��������� 
	cout<<"RowNum"<<csv_file.GetRowNum()<<"  ColumnNum:"<<csv_file.GetColumnNum()<<endl;
	//��ȡ������� 
	csv_file.ReadBlock(strbuff, 0, 1);
	//�޸ı������ 
	csv_file.WriteBlock("cm", 0, 0);
	csv_file.InsertRow(0, true);
	csv_file.InsertColumn(1, false);
	//�����޸ı���ļ� 
	csv_file.SaveFile();
	//�ر��ļ� 
	csv_file.Close(); 
	
	cin >> d;
	return 0;
}
