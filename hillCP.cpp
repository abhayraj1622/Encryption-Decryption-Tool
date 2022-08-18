#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <bits/stdc++.h>
#include <utility>
#include <fstream>
using namespace std;

class encryptdecrypt{

string message;
string key;
string encrypText = "";
string decrypText = "";

int moduloFunction(int A,int B){
	int result = A%B;
	if(result <0)
		return result+B;
	return result;
}
int convertchar(char c){
	if(c >= 'a' && c<='z')
		return (c-'a')+1;
	else if(c == ' ')
		return 0;
	else if(c == ',')
		return 27;
	else
		return 28;
}

char convertInt(int i){
	if(i==0)
		return ' ';
	else if(i == 27)
		return ',';
	else if(i == 28)
		return '.';
	else 
		return i+'a'-1;
}

friend void encryptFile(string file_name);
friend void decryptFile(string file_name);

public:

void setMessage(string message){
	this->encrypText = "";
	this->decrypText = "";
	this->message = message;
}
void setKey(string key){
	this->key = key;
}

string getEncrypted(){
	return this->encrypText;
}
string getDecrypted(){
	return this->decrypText;
}
void encrypt(){
	int i=0;int col1 = 0;int col2 = 0;
	
	if(message.length()%2 != 0)
		message+=" ";
	
	for(i = 0;i<message.length();i++){
		message[i] = tolower(message[i]);
	}
	
	vector<vector<int>> messageMatrix = vector<vector<int>> (2,vector<int> (message.length()/2));
	for(i = 0;i<message.length();i++){
		if(i%2==0)
			messageMatrix[0][col1++] = this->convertchar(message[i]);
		else
			messageMatrix[1][col2++] = this->convertchar(message[i]);
	}
	
	for(i = 0;i<key.length();i++){
		key[i] = tolower(key[i]);
	}
	col1=0;
	col2=0;
	vector<vector<int>> keyMatrix = vector<vector<int>> (2,vector<int>(2));
	for(i = 0;i<key.length();i++){
		if(i%2==0)
			keyMatrix[0][col1++] = this->convertchar(key[i]);
		else
			keyMatrix[1][col2++] = this->convertchar(key[i]);
	}
	
	int det = (keyMatrix[0][0]*keyMatrix[1][1]) - (keyMatrix[0][1]*keyMatrix[1][0]);

	if(det == 0){
		cout <<"Invalid Key"<<endl;
		return;
	}
	int itrCount = message.length()/2;
	
	for (int i = 0; i < itrCount; i++){
            int temp1 = messageMatrix[0][i] * keyMatrix[0][0] + messageMatrix[1][i] * keyMatrix[0][1];
            encrypText += this->convertInt(temp1 % 29);
            int temp2 = messageMatrix[0][i] * keyMatrix[1][0] + messageMatrix[1][i] * keyMatrix[1][1];
            encrypText += this->convertInt(temp2 % 29);
        }	   	
}

void decrypt(){
	int i=0;int col1 = 0;int col2 = 0;
	
	if(message.length()%2 != 0)
		message+=" ";
	
	for(i = 0;i<message.length();i++){
		message[i] = tolower(message[i]);
	}
	
	vector<vector<int>> messageMatrix = vector<vector<int>> (2,vector<int> (message.length()/2));
	for(i = 0;i<message.length();i++){
		if(i%2==0)
			messageMatrix[0][col1++] = this->convertchar(message[i]);
		else
			messageMatrix[1][col2++] = this->convertchar(message[i]);
	}
	
	for(i = 0;i<key.length();i++){
		key[i] = tolower(key[i]);
	}
	col1=0;
	col2=0;
	vector<vector<int>> keyMatrix = vector<vector<int>> (2,vector<int>(2));
	for(i = 0;i<key.length();i++){
		if(i%2==0)
			keyMatrix[0][col1++] = this->convertchar(key[i]);
		else
			keyMatrix[1][col2++] = this->convertchar(key[i]);
	}
	
	int det = (keyMatrix[0][0]*keyMatrix[1][1]) - (keyMatrix[0][1]*keyMatrix[1][0]);

	if(det == 0){
		cout <<"Invalid Key"<<endl;
		return;
	}
	det = this->moduloFunction(det,29);
	    int mulInv = -1;
    for (int i=0; i<29; i++){
        int tempInv = det * i;
        if (this->moduloFunction(tempInv, 29) == 1){
            mulInv = i;
            break;
        } else {
            continue;
        }
    }
    swap(keyMatrix[0][0],keyMatrix[1][1]);
    keyMatrix[0][1] *= -1;
    keyMatrix[1][0] *= -1;

    keyMatrix[0][1] = this->moduloFunction(keyMatrix[0][1], 29);
    keyMatrix[1][0] = this->moduloFunction(keyMatrix[1][0], 29);
    
    for (i = 0; i < 2; i++){
        for (int j = 0; j < 2; j++){
            keyMatrix[i][j] *= mulInv;
        }
    }
    for (i=0; i<2;i++){
        for(int j =0; j <2; j++){
            keyMatrix[i][j] = this->moduloFunction(keyMatrix[i][j], 29);
        }
    }
    
    int itrCount = message.length()/2;
    
    for (int i = 0; i < itrCount; i++){
            int temp1 = messageMatrix[0][i] * keyMatrix[0][0] + messageMatrix[1][i] * keyMatrix[0][1];
            decrypText += this->convertInt(temp1 % 29);
            int temp2 = messageMatrix[0][i] * keyMatrix[1][0] + messageMatrix[1][i] * keyMatrix[1][1];
            decrypText += this->convertInt(temp2 % 29);
        }
}
};
class fileEncDec:public encryptdecrypt{
public:
	void encryptFile(string file_name){
			string line;
			ifstream readFrom(file_name);
			ofstream encryptto("encrypt.txt");
			while(!readFrom.eof()){
				getline(readFrom,line);
				setMessage(line);
				encrypt();
				encryptto << getEncrypted() <<endl;
			}
			
			readFrom.close();
			encryptto.close();
	}
	
	void decryptFile(string file_name){
			string line;
			ifstream readFrom(file_name);
			ofstream decryptto("decrypt.txt");
			while(!readFrom.eof()){
				getline(readFrom,line);
				setMessage(line);
				decrypt();
				decryptto << getDecrypted() <<endl;
			}
			
			readFrom.close();
			decryptto.close();
	}
};
int main(){
	encryptdecrypt H;
	fileEncDec F;
	string t;string m;string k;
	int check =1;
	cout << "\n\t\t\t\tFile and Text Encryption/Decryption\n"<< endl;
	cout << "\t\t\t\t Instructions" << endl;
	cout << "\t\t * Enter character 'e' to encrypt a text message.\n\t\t * Enter character 'd' to decrypt a text message.\n\t\t * * Enter 'ef' to encrypt a text file. \n\t\t\t\t* Enter character 'df' to decrypt a text file.\n\t\t\t\t* Enter character 't' to terminate program. \n\t\t\t\t* Wrong key may lead to incorrect encryption or decryption.\n\t\t\t\t* Enter file name with extension."<<endl;
	
	while(check){
		getline(cin,t);
		if(t=="e"){
				cout << "Enter the message to be encrypted:"<< endl;
				getline(cin,m);
				H.setMessage(m);
				cout << "Enter key for encryption:"<< endl;
				getline(cin,k);
				H.setKey(k);
				H.encrypt();
				cout << "The encrypted message is :"<<endl;
				cout << H.getEncrypted() << endl;
		}
		else if (t =="d"){
				cout << "Enter the message to be decrypted:"<< endl;
				getline(cin,m);
				H.setMessage(m);
				cout << "Enter key for decryption:"<< endl;
				getline(cin,k);
				H.setKey(k);
				H.decrypt();
				cout << "The decrypted message is :"<<endl;
				cout << H.getDecrypted() << endl;
		}
		else if(t == "ef"){
				cout << "Enter file (name) to be encrypted:"<<endl;
				getline(cin,m);
				cout << "Enter key for encryption:"<<endl;
				getline(cin,k);
				F.setKey(k);
				F.encryptFile(m);
				cout << "The message in the given file has been encrypted and stored in encrypt.txt"<<endl;
		}
		else if(t == "df"){
				cout << "Enter file (name) to be decrypted:"<<endl;
				getline(cin,m);
				cout << "Enter key for decryption:"<<endl;
				getline(cin,k);
				F.setKey(k);
				F.decryptFile(m);
				cout << "The message in the given file has been decrypted and stored in decrypt.txt"<<endl;
		}
		else{
				check =0;
				cout << "Program terminated"<<endl;
		}
	}
	return 0;
}
