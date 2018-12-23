#include <sstream>
#include <iostream>
#include <string.h>
#include "csv_access.hpp"

CSVFileAccess::CSVFileAccess(void) 
	:bfile_read(false),
	 bfile_modify(false)
{
	
}

CSVFileAccess::~CSVFileAccess()
{
	
}

/*���ļ����ڵ���������������csv�ļ�ǰ���ȵ��ô˷������ļ�
 *�ļ�������򿪲���ȡ�ļ����ݣ��ļ����������½�һ�� 
 */
int CSVFileAccess::Open(const char *pfilename)
{
	string strline;
	string blockstr;
	stringstream ss;
	vector<string> BlockLine;
	
	file_read.open(pfilename, ios::in);
	
	if (!file_read.is_open()) 
	{
		file_write.open(pfilename, ios::out);
		
		if (!file_write.is_open())
			return -1;
	}
		
	while (getline(file_read, strline))
	{
//		cout<<strline<<endl;
		ss.str(strline);
		while (getline(ss, blockstr, ',')) 
		{
			BlockLine.push_back(blockstr);
		}
		
		BlockLines.push_back(BlockLine);
		BlockLine.clear();
		ss.clear();
	}
	
	if (file_read.is_open()) 
		file_read.close();
	file_path.assign(pfilename);
	bfile_read = true;
		
	return 0;
}

/*�ر��ļ������ļ�������ɺ���ô˷����رմ򿪵��ļ� 
 *Ĭ�Ϲر��ļ�ͬʱ�ᱣ���ļ��޸ģ����� bsave_fileΪfalseʱ���������޸� 
 */
int CSVFileAccess::Close(bool bsave_file)
{
	SaveFile();
	if (file_read.is_open()) 
		file_read.close();
	if (file_write.is_open()) 
		file_write.close();
}

/*�����ļ������ô˷��������ļ��޸� 
 *
 */
int CSVFileAccess::SaveFile(void)
{
	int i, j;
	
	if (!bfile_read)
		return -3;
	if (!bfile_modify)
		return 0;
	
	if (!file_write.is_open()) 
	{
		file_write.open(file_path.c_str(), ios::out | ios::trunc);
		
		if (!file_write.is_open())
			return -1;
	}
	
	for (i = 0; i < BlockLines.size(); i++) 
	{
		for(j = 0; j < BlockLines[0].size(); j++)
		{
			file_write << BlockLines[i][j] << ",";
		}
		file_write << endl;
	}
}

/*����ļ����ݵ�����̨
 *
 */
void CSVFileAccess::PrintFile(void) 
{
	int i, j;
	
	for (i = 0; i < BlockLines.size(); i++) 
	{
		for(j = 0; j < BlockLines[0].size(); j++)
		{
			cout<<BlockLines[i][j] << ",";
		}
		cout << endl;
	}
}

/*������д��ָ�����б���� 
 *csv�ļ����ı���ʽ�ļ������д�������ֻ�����ı��ַ��� 
 */
int CSVFileAccess::WriteBlock(const char *pstr, int row, int column)
{
	string blockstr;
	vector<string> BlockLine;
	int i = 0;
	
	if (!bfile_read)
		return -3;
	
	while(BlockLines.size() <= row)
	{
		BlockLines.push_back(BlockLine);
		
	}
	
	for (i = 0; i < BlockLines.size(); i++)
	{
		while (BlockLines[i].size() <= column)
		{
			BlockLines[i].push_back(blockstr);
		}
	}
	
	BlockLines[row][column].assign(pstr);
	
	bfile_modify = true;

	return 0;
}

/*���ָ�����б������� 
 *
 */
int CSVFileAccess::ClearBlock(int row, int column)
{
	if (!bfile_read)
		return -1;
	
	if (BlockLines.size() <= row)
		return -2;
	
	if (BlockLines[0].size() <= column)
		return -3;
		
	BlockLines[row][column].assign("");
}

/*��ȡָ�����еı������ 
 * ��ȡ���������ı��ַ��� 
 */
int CSVFileAccess::ReadBlock(char *pstr, int row, int column)
{
	string blockstr;
	
	if (!bfile_read)
		return -1;
	
	if (BlockLines.size() <= row)
		return -2;
	
	if (BlockLines[0].size() <= column)
		return -3;
	
	blockstr = BlockLines[row][column];
	strcpy(pstr, blockstr.c_str());
	
	return 0;
}

/*��ָ����ǰ/�����һ������ 
 *
 */	
int CSVFileAccess::InsertRow(int row, bool bpos_after)
{
	vector<string> BlockLine;
	string blockstr;
	int column_num;
	
	if (!bfile_read)
		return -1;
		
	if (BlockLines.size() == 0 && row == 0) 
	{
		BlockLines.push_back(BlockLine);
		return 0;
	}
	
	if (BlockLines.size() <= row)
		return -2;
	
	if (bpos_after)
		row++;
	column_num = BlockLines[0].size();
	
	BlockLines.insert(BlockLines.begin() + row, BlockLine);
	
	while (BlockLines[row].size() < column_num)
	{
		BlockLines[row].push_back(blockstr);
	}
	
	bfile_modify = true;
	
	return 0;
}	

/*��ָ����ǰ/�����һ������ 
 *
 */	
int CSVFileAccess::InsertColumn(int column, bool bpos_after)
{
	vector<string> BlockLine;
	string blockstr;
	int i;
	
	if (!bfile_read)
		return -1;
	
	if (BlockLines.size() == 0) 
		return -2;
	
//	if (column == 0 && BlockLines[0].size() == 0) 
//	{
//		BlockLines[0].push_back(blockstr);
//		return 0;
//	}
	
	if (BlockLines[0].size() <= column && column != 0)
		return -3;
	
	if (bpos_after && BlockLines[0].size() != 0)
		column++;
	
	for (i=0; i<BlockLines.size(); i++)
		BlockLines[i].insert(BlockLines[i].begin() + column, blockstr);
		
	bfile_modify = true;
	return 0;
}

/*ɾ��CSV����ָ��һ�� 
*
*/	
int CSVFileAccess::DeleteRow(int row)
{
	
}

/*ɾ��CSV����ָ��һ�� 
*
*/
int CSVFileAccess::DeleteColumn(int column)
{
	
}

/*��ȡCSV��������� 
*
*/
int CSVFileAccess::GetRowNum(void)
{
	int ret;
	
	ret = BlockLines.size();
	return ret;
}

/*��ȡCSV��������� 
*
*/
int CSVFileAccess::GetColumnNum(void)
{
	int ret;
	
	if (BlockLines.size() < 1)
		return 0;
		
	ret = BlockLines[0].size();
	return ret;
}


