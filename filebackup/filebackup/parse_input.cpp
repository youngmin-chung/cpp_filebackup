/*
Name:		Youngmin Chung
Date:		Jan 24, 2019
Purpose:	To implement major functions
Version:	1.0
*/

#include "pch.h"
using namespace std;

//function to parse input data to a container
deque<string> parseInput(int argc, char * argv[])
{
	deque<string> dArg;
	for (int i = 1; i < argc; ++i)
		dArg.push_back(argv[i]);

	return dArg;
}


//function to scan and parse the current folder and all its subfolders
void report(map<char, int> & mSwitches, string strExpression, regex expression, path const& folder, vector<Filebackup> & vFilebackup)
{
	//Create a directory iterator passing it the folder object
	directory_iterator dir_it(folder);

	//Create another iterator to end
	directory_iterator dir_end;

	//read directory information and begin the search
	while (dir_it != dir_end)
	{
		try
		{
			if (is_regular_file(dir_it->status()))
			{
				//read folders' data and insert corresponding data to vector
				vector<Filebackup>::iterator it = vFilebackup.begin();

				for (; it != vFilebackup.end(); ++it)
				{
					if (dir_it->path().filename().string() == it->fileName && dir_it->path().parent_path().string() == it->folderName)
						break;
				}

				if (it != vFilebackup.end())
				{
					it->fileCount++;
					it->filesizeCount += file_size(dir_it->path());
					it->fileNumPerFolder++;
				}
				else
				{
					/*ExtManager extRec(dir_it->path().filename().string(), 1, file_size(dir_it->path()));
					vExtManager.push_back(extRec);*/
					regex reg(".(c|cpp|h|hpp)");
					if (regex_match(dir_it->path().filename().string(), reg))
					{
						Filebackup fileRec(dir_it->path().filename().string(), 1, 1, file_size(dir_it->path()), dir_it->path().parent_path().string());
						vFilebackup.push_back(fileRec);
					}
				}
			}

		}
			
		catch (filesystem_error & ex)
		{
			cerr << ex.what() << endl;
		}
		++dir_it;
	}

	sort(vFilebackup.begin(), vFilebackup.end(), byExtAscending);

	string options = "";
	string extPrint;
	extPrint = ".*";

	//print detail report and summary
	unsigned long totalExtCount = 0;
	unsigned long long totalFileSize = 0;

	//cout << options << " ext: " << extPrint << " folder: " << folder << endl;
	cout << "Root Directory: " << current_path();
	cout << endl;

	for (auto x : vFilebackup)
	{
		cout << setw(27) << x.fileName << right << setw(12) << "Size: " << x.filesizeCount << endl;

		totalExtCount += x.fileCount;
		totalFileSize += x.filesizeCount;
	}
	if (totalExtCount == 0)
		cout << "\nThere is no files(.c/.cpp/.h//hpp)\n" << endl;
	cout << "Backup Total: " << setw(13) << totalExtCount << setw(12) << "Size: " << totalFileSize << endl;

	cout << endl;
}


