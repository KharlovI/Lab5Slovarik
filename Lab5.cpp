#include <iostream>
#include <string>
#include <fstream>
#include <Windows.h>

struct Data
{
	std::string englWord;

	std::string rusWord;

	int countOfChecking;

	Data(std::string** words, int countOfChecking , int pointer)
	{
		this->englWord = words[pointer][0];

		this->rusWord = words[pointer][1];

		this->countOfChecking = countOfChecking;
	}
};

struct Node
{
	Data* nodeData;

	Node* l;

	Node* r;

	Node(Data* nodeData)
	{
		this->nodeData = nodeData;
	}
};

bool checkIfFirstStringIsLoverThenSecond(std::string str1, std::string str2)
{
	for (int i = 0; ; i++)
	{
		if (str1[i] == NULL)
		{
			return 0;
		}

		if (str2[i] == NULL)
		{
			return 1;
		}
		if (tolower(str1[i]) > tolower(str2[i]))
		{
			return 1;
		}

		else if (tolower(str1[i]) < tolower(str2[i]))
		{
			return 0;
		}
	}

	return 1;
}

std::string** sortByAlphabet(std::string** str, int size, int* counts)
{
	for (int i = 0; i < size - 1; i++)
	{
		for (int j = i + 1; j < size; j++)
		{
			if (checkIfFirstStringIsLoverThenSecond(*str[i], *str[j]))
			{
				std::swap(str[i], str[j]);

				std::swap(counts[i], counts[j]);
			}
		}
	}

	return str;
}

void addNodeToTree(Node** T, std::string** words, int pointer, int* counts, int size, bool ebal) // count - количество вызовов слов  size - общее количество слов 
{
	Node* newNode = new Node(new Data(words, counts[pointer] , pointer));

	if ((*T) == nullptr)
	{
		(*T) = newNode;

		(*T)->l = nullptr;

		(*T)->r = nullptr;

		if (!ebal)
		{
			return;
		}
	}

	if (ebal)
	{
		if (pointer != size - 1)
		{			
		    addNodeToTree(&(*T)->r, words, ++pointer, counts, size, ebal);
		}
	}

	else
	{
		addNodeToTree(&(*T)->l, words, pointer, counts, size, ebal);
	}

	return;
}

void ToNewTree(Node** newT, Node** oldT)
{	
	Node* newNode = new Node((*oldT)->nodeData);

	if ((*newT) == nullptr)
	{
		(*newT) = newNode;

		(*newT)->l = nullptr;

		(*newT)->r = nullptr;
	}

	else
	{
		if ((*newT)->nodeData->countOfChecking <= (*oldT)->nodeData->countOfChecking)
		{
			ToNewTree(&(*newT)->r, oldT);
		}

		else
		{
			ToNewTree(&(*newT)->l, oldT);
		}
	}
}

void deleteOldTreeAndGoToNewTree(Node** oldT, Node** newT, int maxValue)
{
	if (*oldT == nullptr)
	{
		return;
	}

	if ((*oldT)->r != nullptr)
	{
		if ((*oldT)->r->nodeData->countOfChecking >= maxValue)
		{
			ToNewTree(newT, &(*oldT)->r);

			deleteOldTreeAndGoToNewTree(&(*oldT)->r, newT,(*oldT)->r->nodeData->countOfChecking);

			delete((*oldT)->r);

			(*oldT)->r = nullptr;
		}

		else
		{
			ToNewTree(newT, &(*oldT)->r);

			deleteOldTreeAndGoToNewTree(&(*oldT)->r, newT, maxValue);

			delete((*oldT)->r);

			(*oldT)->r = nullptr;
		}
	}

	if ((*oldT)->l != nullptr)
	{
		if ((*oldT)->l->nodeData->countOfChecking >= maxValue)
		{
			ToNewTree(newT, &(*oldT)->l);

			deleteOldTreeAndGoToNewTree(&(*oldT)->l, newT, (*oldT)->l->nodeData->countOfChecking);

			delete((*oldT)->l);

			(*oldT)->l = nullptr;
		}

		else
		{
			ToNewTree(newT, &(*oldT)->l);

			deleteOldTreeAndGoToNewTree(&(*oldT)->l, newT, maxValue);

			delete((*oldT)->l);

			(*oldT)->l = nullptr;
		}
	}
}

void onlyDelete(Node** T)
{
	if (*T == nullptr)
	{
		return;
	}

	if ((*T)->r != nullptr)
	{
		onlyDelete(&(*T)->r);

		delete((*T)->r);

		(*T)->r = nullptr;
	}

	if ((*T)->l != nullptr)
	{
		onlyDelete(&(*T)->l);

		delete((*T)->l);

		(*T)->l = nullptr;
	}
}

void printWords(Node** T, int language) // language: 1 - engl, 2 - rus
{
	if ((*T) != nullptr)
	{
		if (language == 1)
		{
			std::cout << (*T)->nodeData->englWord;
		}

		else
		{
			std::cout << (*T)->nodeData->rusWord;
		}

		std::cout << std::endl;

		printWords(&(*T)->r, language);

		printWords(&(*T)->l, language);
	}

	else
	{
		return;
	}
}

void printTree(Node** T)
{
	if ((*T) != nullptr)
	{
		std::cout << (*T)->nodeData->englWord << "    " << '\t' << (*T)->nodeData->rusWord << " : " << (*T)->nodeData->countOfChecking;

		std::cout << std::endl;

		if ((*T)->r!=nullptr)
		{
			printTree(&(*T)->r);
		}
		if ((*T)->l != nullptr)
		{
			printTree(&(*T)->l);
		}
	}

	else
	{
		return;
	}
}

void findWordInTree(Node** T, std::string enteredWord, int answer)
{
	switch (answer)
	{
	case 1:
		if ((*T) == nullptr)
		{
			break;
		}
		if (enteredWord == (*T)->nodeData->englWord)
		{
			std::cout << enteredWord << " - " << (*T)->nodeData->rusWord;

			std::cout << std::endl;

			(*T)->nodeData->countOfChecking++;

			break;
		}

		else
		{
			findWordInTree(&(*T)->r, enteredWord, answer);

			findWordInTree(&(*T)->l, enteredWord, answer);
		}
		break;

	case 2:
		if ((*T) == nullptr)
		{
			break;
		}
		if (enteredWord == (*T)->nodeData->rusWord)
		{
			std::cout << enteredWord << " - " << (*T)->nodeData->englWord;

			std::cout << std::endl;

			(*T)->nodeData->countOfChecking++;

			break;
		}

		else
		{
			findWordInTree(&(*T)->r, enteredWord, answer);

			findWordInTree(&(*T)->l, enteredWord, answer);
		}
	}
}

std::string findNewWordInTree(Node** T, std::string enteredWord, int answer)
{
	switch (answer)
	{
	case 1:
		if ((*T) == nullptr)
		{
			return enteredWord;
		}
		if (enteredWord == (*T)->nodeData->englWord)
		{
			return "This word are alredy added";

			break;
		}

		else
		{
			return findNewWordInTree(&(*T)->r, enteredWord, answer);

			return findNewWordInTree(&(*T)->l, enteredWord, answer);
		}

		break;

	case 2:
		if ((*T) == nullptr)
		{
			return enteredWord;
		}
		if (enteredWord == (*T)->nodeData->englWord)
		{
			return "This word are alredy added";

			break;
		}

		else
		{
			return findNewWordInTree(&(*T)->r, enteredWord, answer);

			return findNewWordInTree(&(*T)->l, enteredWord, answer);
		}

		break;
	}
}

std::string** getWordsFromFile(int size)
{
	std::string** words = new std::string*[size];

	for (int i = 0; i < size; i++)
	{
		words[i] = new std::string[2];
	}

	std::ifstream fileWords;

	fileWords.open("D:\\Lab5\\Words.txt");

	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			std::getline(fileWords, words[i][j]);
		}
	}

	fileWords.close();

	return words;
}

int getSizeFromFile()
{
	std::ifstream f;

	f.open("D:\\Lab5\\Size.txt");

	std::string buffer;

	std::getline(f, buffer);

	int count = std::stoi(buffer);

	f.close();

	return count;
}

int* getCountFromFile(int size)
{
	std::ifstream f;

	f.open("D:\\Lab5\\Count.txt");

	std::string* buffer = new std::string[size];

	for (int i = 0; i < size; i++)
	{
		std::getline(f, buffer[i]);
	}

	int* count = new int[size];

	for (int i = 0; i < size; i++)
	{
		count[i] = stoi(buffer[i]);
	}

	f.close();

	return count;
}

int chouseLanguage()
{
	std::cout << "Chose language:" << std::endl;

	std::cout << "English - 1" << '\t' << "Russian - 2" << std::endl;

	std::string answer;

	std::cin >> answer;

	while (answer != "2" && answer != "1")
	{
		std::cout << "Pleas, chose 1 or 2" << std::endl;

		std::cin >> answer;
	}

	int answer2 = std::stoi(answer);

	return answer2;
}

void addNewWord(Node**T, int *size)
{
	std::cout << "Pleas, enter english word:" << std::endl;

	std::string englWord;

	std::cin >> englWord;

	std::cout << "Pleas, enter russian word:" << std::endl;

	std::string rusWord;

	std::cin >> rusWord;

	std::string englanswer;

	englanswer = findNewWordInTree(&(*T), englWord, 1);

	std::string rusanswer;

	rusanswer = findNewWordInTree(&(*T), rusWord, 1);

	if (englanswer == englWord && rusanswer == rusWord)
	{
		(*size) += 1;

		std::string** words = new std::string*[1];

		words[0] = new std::string[2];

		words[0][0] = englWord;

		words[0][1] = rusWord;

		int* a = new int[1];

		a[0] = 1;

		addNodeToTree(&(*T), words, 0, a, *size, 0);
	}

	else
	{
		std::cout << englanswer << std::endl;
	}
}

void NеwTreeToFile(Node** newT, std::ofstream *WordsFile, std::ofstream* CountsFile)
{
	if (newT == nullptr)
	{
		return;
	}

	if ((*newT)->r != nullptr)
	{
		*WordsFile << (*newT)->r->nodeData->englWord << std::endl;

		*WordsFile << (*newT)->r->nodeData->rusWord << std::endl;

		*CountsFile << (*newT)->r->nodeData->countOfChecking << std::endl;

		NеwTreeToFile(&(*newT)->r, WordsFile, CountsFile);
	}

	if ((*newT)->l != nullptr)
	{
		*WordsFile << (*newT)->l->nodeData->englWord << std::endl;

		*WordsFile << (*newT)->l->nodeData->rusWord << std::endl;

		*CountsFile << (*newT)->l->nodeData->countOfChecking << std::endl;

		NеwTreeToFile(&(*newT)->l, WordsFile, CountsFile);
	}
}

void sizeToFile(int size)
{
	std::ofstream f;

	f.open("D:\\Lab5\\Size.txt");

	f << size;

	f.close();
}

void Menu()
{
	bool ans = 1;

	int ans2 = 0;

	Node* oldTSortByLetters = nullptr;

	Node* oldTSortByValue = nullptr;

	Node* newT = nullptr;

	int size = getSizeFromFile();

	std::string** buffer = getWordsFromFile(size);

	int* counts = new int[size];

	counts = getCountFromFile(size);

	addNodeToTree(&oldTSortByValue, buffer, 0, counts, size, 1);

	sortByAlphabet(buffer, size, counts);

	addNodeToTree(&oldTSortByLetters, buffer, 0, counts, size, 1);

	while (true)
	{
		std::cout << "Translate word - 1" << '\t' << "show english words - 2" << '\t' << '\t' << "Show russian words - 3" << std::endl;
		
		std::cout << "Add new word - 4"  << '\t' << "If you want to finish - 0";

		std::cout << std::endl;

		std::cin >> ans2;

		while (ans2 > 5 || ans2 < 0)
		{
			std::cout << "Pleas, chouse correct answer:" << std::endl;

			std::cin >> ans2;
		}

		switch(ans2)
		{
		    case 0:
				break;

			case 1:

				while (ans)
				{
					int answer = chouseLanguage();

					std::cout << "Enter word:" << std::endl;

					std::string enteredWord;

					std::cin >> enteredWord;

					findWordInTree(&oldTSortByValue, enteredWord, answer);

					std::cout << "Would you like to check one more word?" << std::endl << "IF yes - 1" << '\t' << "no - 0" << std::endl;

					std::cin >> ans;
				}

				std::cout << std::endl;

				ans = 1;

				break;

			case 2:

				printWords(&oldTSortByLetters, 1);

				std::cout << std::endl;

				break;

			case 3:

				printWords(&oldTSortByLetters, 0);

				std::cout << std::endl;

				break;

			case 4:

				while (ans)
				{
					addNewWord(&oldTSortByValue, &size);

					std::cout << "Would you like to add one more word?" << std::endl << "IF yes - 1" << '\t' << "no - 0" << std::endl;

					std::cin >> ans;
				}

				std::cout << std::endl;

				ans = 1;

				break;
		}

		if (ans2 == 0)
		{
			break;
		}
	}

	std::cout << std::endl;

	std::cout << "Old Tree:" << std::endl;

	std::cout << std::endl;

	printTree(&oldTSortByLetters);

	std::cout << std::endl;

	std::cout << "New Tree:" << std::endl;

	ToNewTree(&newT, &oldTSortByValue);

	deleteOldTreeAndGoToNewTree(&oldTSortByValue, &newT, oldTSortByValue->nodeData->countOfChecking);

	delete(&(*oldTSortByValue));

	oldTSortByValue = nullptr;

	printTree(&newT);

	std::cout << std::endl; 

	onlyDelete(&oldTSortByLetters);

	delete(&(*oldTSortByLetters));

	oldTSortByLetters = nullptr;

	std::ofstream WordsFile;

	std::ofstream CountsFile;

	WordsFile.open("D:\\Lab5\\Words.txt");

	CountsFile.open("D:\\Lab5\\Count.txt");

	WordsFile << newT->nodeData->englWord << std::endl;

	WordsFile << newT->nodeData->rusWord << std::endl;

	CountsFile << newT->nodeData->countOfChecking << std::endl;

	NеwTreeToFile(&newT, &WordsFile, &CountsFile);

	sizeToFile(size);

	onlyDelete(&newT);

	delete(&(*newT));

	newT = nullptr;

	WordsFile.close();

	CountsFile.close();
}

int main()
{
	SetConsoleCP(1251);// установка кодовой страницы win-cp 1251 в поток ввода

	SetConsoleOutputCP(1251); // установка кодовой страницы win-cp 1251 в поток вывода

	Menu();
}
