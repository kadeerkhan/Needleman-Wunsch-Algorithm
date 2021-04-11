#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include<fstream>
#include<string>
#include <iomanip>
using namespace std;
//we are dope

void reverse(string &s)//funstion to reverse a string
{
	string n;
	for (int i = s.length() - 1; i >= 0; --i)
	{
		n.push_back(s[i]);
	}
	s = n;
}
fstream out;//file variable for output
string r1 = "", r2 = "";//string for aligned songs
char** tb;//traceback matrix variable
int** score;//score matrix variable
//temporary variables
char* sn2 = NULL;
char* sid2 = NULL;
char* sid1 = NULL;
char* sn1 = NULL;
//////////////////////
//variables for songname and song id
string songname1;
string songname2, songid1, songid2;

void needlman(string s1, string s2)
{
	int match = 1,								//reward for a match while calculating score
		mismatch = -1,							//penalty for a missmatch
		gap = -2;								//penalty for moving down or right
	int l1 = s1.length() + 1;
	int l2 = s2.length() + 1;
	tb = new char* [l2];//traceback matrix
	for (int i = 0; i < l2; ++i)
	{
		tb[i] = new char[l1];
	}

	score = new int* [l2];//score matrix
	for (int i = 0; i < l2; ++i)
	{
		score[i] = new int[l1];
	}
	score[0][0] = 0;
	tb[0][0] = 'o';
	for (int i = 1; i < l2; ++i)
	{
		score[i][0] = score[i - 1][0] + gap;//initatializing the score array for string 1 from left to right
		tb[i][0] = 'u';
	}
	for (int i = 1; i < l1; ++i)
	{
		score[0][i] = score[0][i - 1] + gap;//initializing the score array for string 2 from top to down
		tb[0][i] = 'l';
	}


	for (int i = 1; i < l2; ++i)
	{
		for (int j = 1; j < l1; ++j)
		{
			int sl, su, sd;//variables for calculating the score sl:if you have moved rightward, su:if you have moved downward, sd:if you have moved diagbally
			if (s2[i - 1] == s1[j - 1])
			{
				sd = score[i - 1][j - 1] + match;
			}
			else
			{
				sd = score[i - 1][j - 1] + mismatch;
			}
			su = score[i - 1][j] + gap;
			sl = score[i][j - 1] + gap;
			if (sd > sl)//comparing all 3 variables
			{
				if (sd > su)
				{//sd>sl&&sd>su
					score[i][j] = sd;
					tb[i][j] = 'd';
				}
				else
				{//su>sd>sl
					score[i][j] = su;
					tb[i][j] = 'u';
				}
			}
			else
			{
				if (sl > su)
				{//sl>sd&&sl>su
					score[i][j] = sl;
					tb[i][j] = 'l';
				}
				else
				{//su>sl>sd
					score[i][j] = su;
					tb[i][j] = 'u';
				}
			}
		}
	}//writing the alignment matrix on the output file 
	for (int i = 0; i < l2; ++i)
	{
		if (i > 0)
		{
		out << right << setw(5) << s2[i - 1];
		}
		else
		{
			for (int e = 0; e < l1+1; ++e)
			{
				if(e==0)
				{
					out << right << setw(5) << " ";
				}
				else
				{
					if (e == 1)
					{
						out << right << setw(5) << "_";
					}
					else
					{
						out << right << setw(5) << s1[e - 2];
					}
				}
			}
			out << endl;
		}
		for (int j = 0; j < l1; ++j)
		{
			if (i == 0&&j==0)
			{
				out << right << setw(5) << "_";
			}
			out  << right << setw(5) << score[i][j] ;
		}
		out << endl<<endl;
	}
	for (int i = l2-1 , j = l1-1 ;( i >= 0)&& (j >= 0); )//loop for tracing back the path from traceback matrix
	{
		//cout << tb[i][j] << endl;
		if (i == 0 && j == 0)
		{
		//nothing
			break;
		}
		else
		{
			if (tb[i][j] == 'd')//if you have moved diagonally
			{
				r1.push_back(s1[j - 1]);
				r2.push_back(s2[i - 1]);
				--i;
				--j;
			}
			else
			{
				if (tb[i][j] == 'u')//if you have moved downward
				{
					r2.push_back(s2[i - 1]);
					r1.push_back('_');
					--i;
				}
				else
				{
					if (tb[i][j] == 'l')//if you have moved rightward
					{
						r2.push_back('_');
						r1.push_back(s1[j - 1]);
						--j;
					}
				}
			}
		}
	}
	//revrsing the strings of aligned songs
	reverse(r1);
	reverse(r2);
	//deleting the memory on heap
	for (int i = 0; i < l2; ++i)
	{
		delete tb[i];
	}
	delete[]tb;
	for (int i = 0; i < l2; ++i)
	{
		delete score[i];
	}
	delete[]score;

}


int main()
{
	fstream file;//file variable for first file
	out.open("out.txt", ios::out );//opening the output file in writing mode
	out << "Aligment Matrix\n";
	string in;
	cout << "Enter the name of 1st file: ";
	cin >> in;
	file.open(in);//opening in read mode
	string content,line;
	if (file.is_open())
	{
		getline(file, line);
		sn1 =(char*) line.c_str();
		char *tmp1=strtok(sn1,">");
		sn1 = strtok(tmp1, "|");
		sid1 = strtok(NULL, "|");
		songid1 = sid1;
		songname1 = sn1;
		while (getline(file,line))
		{
			content +=line;
		}
	}
	file.close();//1st file closed
	cout << "\nThe content of 1st song is: " << content<<endl;
	fstream file2;//file variable for 2nd file
	string in2;
	cout << "Enter the name of 2nd file: ";
	cin >> in2;
	file2.open(in2); //opening in write mode
	string content2, line2;
	
	if (file2.is_open())
	{
		getline(file2, line2);
		sn2 = (char*)line2.c_str();
		char* tmp2 = strtok(sn2, ">");//splitting the string
		sn2 = strtok(tmp2, "|");
		sid2 = strtok(NULL, "|");
		songid2 = sid2;
		songname2 = sn2;
		while (getline(file2, line2))
		{
			content2 += line2;
		}
	}
	file2.close();//2nd file closed
	cout << "\nThe content of 2nd song is: " << content2 << endl;
	needlman(content, content2);
	int match=0,gap=0,mismatch=0;
	for (int i = 0; i < r1.length(); ++i)//loop for matching the strings
	{
		if (r1[i] == r2[i])
		{
			++match;
		}
		else
		{
			if ((r1[i]=='_') || (r2[i]=='_'))
			{
				++gap;
			}
			else
			{
				++mismatch;
			}
		}
	}
	//wrting the song about songs information 
	out << songid1<<"\t";
	out << left << setw(5) << songname1;
	out << "\t:" << left << setw(10) << r1 << endl;
	out << songid2<<"\t";
	out<< left << setw(5) << songname2;
	out<< "\t:" << left << setw(10) << r2<<endl;
	out << endl << endl;
	float p =(float) match / (float)r1.length() * (float)100;//taking the perstage of final result
	p = round(p);//rounding off the result
	if (p < 67)
	{
		out << "The similarity between the two songs was " << p << "% and is below the similarity threshold of 67%. Thus, no copyright infringement has occurred.";
	}
	else
	{
		out << "The similarity between the two songs was " << p << "% and is not below the similarity threshold of 67%. Thus, copyright infringement has occurred.";
	}
	out.close();
	return 0;
}