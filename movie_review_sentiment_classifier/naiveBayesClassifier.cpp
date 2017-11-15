#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <fstream>
#include <bits/stdc++.h>
using namespace std;

int arr[90000][4];
long double prob[90000][4];
int cnt =0;
int stop_cnt = 174;
int psum  = 0, nsum = 0,bpsum=0 , bnsum=0;
int SZ = 89527;
void input();
void sums();
void populate();
void test();
void test2();
void test3();
void input_vocab();
void stopwords();
void stopwords2();
map<string, int> M;

int main(){
	cout<<"Training using given dataset..."<<endl;
    	input(); //takes input from the training dataset
	sums(); //computes the total number of word occurrences 
	//cout<<psum<<endl;
	//cout<<nsum<<endl;
	populate(); //precomputes the conditional probabilities and stores them in a global array called prob
	//for(int i =0 ;i<100;i++){
	//	for(int j=0;j<4;j++){
	//	cout<<prob[i][j]<<" ";
	//	}
	//	cout<<endl;	
	//}
	cout<<"Training done."<<endl;
	cout<<endl;
	cout<<"Using Naive Bayes Classifier for classification..."<<endl;
	test(); //reads testset and tests using naive bayes classifier
	cout<<endl;
	cout<<"Using Binary Naive Bayes Classifier for classification..."<<endl;
	test3(); //reads testset and tests using binary naive bayes classifier
	cout<<endl;
	cout<<"Removing stopwords."<<endl;
	input_vocab(); //reads the movie review vocabulary into a dictionary
	stopwords(); //handles the stopwords by changing arrays appropriately
	cout<<"Recomputing probabilities after removing stopwords."<<endl<<endl;
	populate(); //again calculates conditional probability after removing stopwords
	stopwords2(); //changes the probability of stopwords to 1, and log normalises to avoid NaN
	SZ = SZ - stop_cnt;
	psum  = 0; nsum = 0;bpsum=0 ; bnsum=0;
	sums(); //recalculates total word occurrences 
	cout<<"Using Naive Bayes Classifier for classification post stopword removal"<<endl;
	test(); //tests again after stopword removal using naive bayes classifier
	cout<<endl;
	cout<<"Using Binary Naive Bayes Classifier for classification post stopword removal"<<endl;
	test3(); //tests again after stopword removal using binary naive bayes classifier
    return 0;
}

void input_vocab(){    //WORKING
	ifstream fin("imdb.vocab",ios::in);
	int i = 0;
        string line;
	while(fin>>line){
		M[line] = i;
		//cout<<line<<endl;
		i++;
	}
	//cout<<i<<endl;
	fin.close();
}

void stopwords(){   //WORKING
	ifstream fin("stop",ios::in);
        string line;
	while(fin>>line){
		arr[M[line]][0] = 0;
		arr[M[line]][1] = 0;
		//cout<<line<<endl;
		arr[M[line]][2] = 0;
		arr[M[line]][3] = 0;
	}
	fin.close();
	//cout<<"PF"<<endl;
}

void stopwords2(){
	ifstream fin("stop",ios::in);
        string line;
	while(fin>>line){
		prob[M[line]][0] = 0;
		prob[M[line]][1] = 0;
		//cout<<line<<endl;
		prob[M[line]][2] = 0;
		prob[M[line]][3] = 0;
	}
	fin.close();
}

void populate(){
	for(int i = 0;i<90000;i++){
		prob[i][0] = log((long double)(arr[i][0]+1)/(SZ + psum));
		prob[i][1] = log((long double)(arr[i][1]+1)/(SZ + nsum));
		prob[i][2] = log((long double)(arr[i][2]+1)/(SZ + bpsum) );
		prob[i][3] = log((long double)(arr[i][3]+1)/(SZ + bnsum));
		//if(arr[i][2]>5)cout<<arr[i][2]<<" "<<arr[i][3]<<endl;
	}
}

void sums(){
	for(int i =0; i<89527;i++){
		psum += arr[i][0];
		nsum += arr[i][1];
		bpsum += arr[i][2];
		bnsum += arr[i][3];
	}
}

void input(){    //WORKING
    freopen("abc/train/labeledBow.feat", "r", stdin);
    string line;
    while(getline(cin, line)){
            cnt++;
		//	cout<<line<<endl;
            //cout<<cnt<<"::\n";
            int a =0;
            int p,q,r,num;
            //cout<<c_str;
			int size = line.length();
			int i;
			string temp="";
			for(i=0;i<size;i++)
			{
				if(line[i]==' ')
				{
					break;
				}
				else
				{
					temp.push_back(line[i]);
				}
			}
			p = atoi(temp.c_str());
			temp="";
			i++;
			//cout<<p<<" ";
			string lp="";
			//cout<<i<<" "<<size;
			while(i<size)
			{
				for(;i<size;i++)
				{
					if(line[i]==' ')
					{
						break;
					}
					else if(line[i]==':')
					{
						lp = temp;
						temp="";
					}
					else
					{
						temp.push_back(line[i]);
					}
				}	
				q = atoi(lp.c_str());
				r = atoi(temp.c_str());
				//cout<<q<<":"<<r<<" ";
				temp="";
				lp="";
				if(p<=4)
				{
					arr[q][1]+=r;
					arr[q][3]+=1;
				}
				else
				{
					arr[q][0]+=r;
					arr[q][2]+=1;
				}
				i++;
			}
			//cout<<endl;
     		     
    }
	//cout<<"Input done."<<endl; 
    return ;
}

void test(){
    ifstream fin("abc/test/labeledBow.feat");
    string line;
    int true_pos = 0, true_neg = 0, false_pos = 0, false_neg = 0;
    //long double pprob = log(0.5),nprob = log(0.5);
    //cout<<pprob<<endl;
    while(getline(fin, line)){	
            cnt++;
			//cout<<line<<endl;
            //cout<<cnt<<"::\n";
		long double pprob = log(0.5),nprob = log(0.5);
            int p,q,r,num;
			int ans;
			int size = line.length();
			
			int i;
			string temp="";
			for(i=0;i<size;i++)
			{
				if(line[i]==' ')
				{
					break;
				}
				else
				{
					temp.push_back(line[i]);
				}
			}
			p = atoi(temp.c_str());
			temp="";
			i++;
			//cout<<p<<" ";
			string lp="";
			while(i<size)
			{
				for(;i<size;i++)
				{
					if(line[i]==' ')
					{
						break;
					}
					else if(line[i]==':')
					{
						lp = temp;
						temp="";
					}
					else
					{
						temp.push_back(line[i]);
					}
				}
				q = atoi(lp.c_str());
				r = atoi(temp.c_str());
				//cout<<q<<":"<<r<<" ";
				temp="";
				lp="";
				for(int a = 0;a<r;a++){
				//if(prob[q][0]==0)cout<<prob[q][0]<<endl;
					pprob += prob[q][0];
					nprob += prob[q][1];				
				}
				i++;	
			//cout<<pprob<<" "<<nprob<<endl;
			}
			if(pprob >= nprob){
				ans = 0; //POSITIVE
			}
			else
				ans = 1;
			//cout<<endl;
			if( p <= 4 && ans ==0) false_pos++;
			if( p <= 4 && ans ==1) true_neg++;
			if( p > 4 && ans == 0) true_pos++;
			if( p > 4 && ans == 1) false_neg++;
			
     		     
    }
fin.close();
//cout<< false_neg<<"\n";
	//cout<<"Input done."<<endl; 
	cout<<"Accuracy is: "<<(1.0*true_pos+true_neg)/(true_pos+true_neg+false_pos+false_neg)<<"."<<endl;
	long double prcs = (long double)(true_pos)/(true_pos+false_pos);
	long double recall = (long double)(true_pos)/(true_pos+false_neg);
	long double f1 = (2*prcs*recall)/(prcs+recall);
	cout<<"Precision is: "<<prcs<<"."<<endl;
	cout<<"Recall is: "<<recall<<"."<<endl;
	cout<<"F1 Score is: "<<f1<< "."<<endl;
    return ;
}


void test3(){
    ifstream fin("abc/test/labeledBow.feat");
    string line;
    int true_pos = 0, true_neg = 0, false_pos = 0, false_neg = 0;
    //long double pprob = log(0.5),nprob = log(0.5);
    //cout<<pprob<<endl;
    while(getline(fin, line)){	
            cnt++;
			//cout<<line<<endl;
            //cout<<cnt<<"::\n";
		long double pprob = log(0.5),nprob = log(0.5);
            int p,q,r,num;
			int ans;
			int size = line.length();
			
			int i;
			string temp="";
			for(i=0;i<size;i++)
			{
				if(line[i]==' ')
				{
					break;
				}
				else
				{
					temp.push_back(line[i]);
				}
			}
			p = atoi(temp.c_str());
			temp="";
			i++;
			//cout<<p<<" ";
			string lp="";
			while(i<size)
			{
				for(;i<size;i++)
				{
					if(line[i]==' ')
					{
						break;
					}
					else if(line[i]==':')
					{
						lp = temp;
						temp="";
					}
					else
					{
						temp.push_back(line[i]);
					}
				}
				q = atoi(lp.c_str());
				r = atoi(temp.c_str());
				//cout<<q<<":"<<r<<" ";
				temp="";
				lp="";
				for(int a = 0;a<1;a++){
				//if(prob[q][0]==0)cout<<prob[q][0]<<endl;
					pprob += prob[q][2];
					nprob += prob[q][3];				
				}
				i++;	
			//cout<<pprob<<" "<<nprob<<endl;
			}
			if(pprob >= nprob){
				ans = 0; //POSITIVE
			}
			else
				ans = 1;
			//cout<<endl;
			if( p <= 4 && ans ==0) false_pos++;
			if( p <= 4 && ans ==1) true_neg++;
			if( p > 4 && ans == 0) true_pos++;
			if( p > 4 && ans == 1) false_neg++;
			
     		     
    }
fin.close();
//cout<< false_neg<<"\n";
	//cout<<"Input done."<<endl; 
	cout<<"Accuracy is: "<<(1.0*true_pos+true_neg)/(true_pos+true_neg+false_pos+false_neg)<<"."<<endl;;
	long double prcs = (long double)(true_pos)/(true_pos+false_pos);
	long double recall = (long double)(true_pos)/(true_pos+false_neg);
	long double f1 = (2*prcs*recall)/(prcs+recall);
	cout<<"Precision is: "<<prcs<<"."<<endl;
	cout<<"Recall is: "<<recall<<"."<<endl;
	cout<<"F1 Score is: "<<f1<< "."<<endl;
    return ;
}



void test2(){
    ifstream fin("abc/test/labeledBow.feat");
    int true_pos = 0, true_neg = 0, false_pos = 0, false_neg = 0;
    
    string line;
    while(getline(fin, line)){
            cnt++;
		long double pprob =log( 0.5), nprob =log( 0.5);
			//cout<<line<<endl;
            //cout<<cnt<<"::\n";
            		int p,q,r,num;
			int ans;
			int size = line.length();
			int i;
			string temp="";
			for(i=0;i<size;i++)
			{
				if(line[i]==' ')
				{
					break;
				}
				else
				{
					temp.push_back(line[i]);
				}
			}
			p = atoi(temp.c_str());
			temp="";
			i++;
			//cout<<p<<" ";
			string lp="";
			while(i<size)
			{
				for(;i<size;i++)
				{
					if(line[i]==' ')
					{
						break;
					}
					else if(line[i]==':')
					{
						lp = temp;
						temp="";
					}
					else
					{
						temp.push_back(line[i]);
					}
				}
				q = atoi(lp.c_str());
				r = atoi(temp.c_str());
				//cout<<q<<":"<<r<<" ";
				temp="";
				lp="";
				cout<<q<<" "<<q<<endl;
				pprob += prob[q][2];
				nprob += prob[q][3];
				i++;
				//cout<<pprob<<endl;	
			}
			cout<<pprob<<" "<<nprob<<endl;
			if(pprob >= nprob){
				ans = 0; //POSITIVE
			}
			else
				ans = 1;
			//cout<<endl;
			if( p <= 4 && ans ==0) false_pos++;
			if( p <= 4 && ans ==1) true_neg++;
			if( p > 4 && ans == 0) true_pos++;
			if( p > 4 && ans == 1) false_neg++;
     		     
			
     		     
    }
fin.close();
	//cout<<"Input done."<<endl;  
	cout<<"Accuracy is: "<<(1.0*true_pos+true_neg)/(true_pos+true_neg+false_pos+false_neg)<<"."<<endl;;	
	long double prcs = (long double)(true_pos)/(true_pos+false_pos);
	long double recall = (long double)(true_pos)/(true_pos+false_neg);
	long double f1 = (2*prcs*recall)/(prcs+recall);
	cout<<"Precision is: "<<prcs<<"."<<endl;
	cout<<"Recall is: "<<recall<<"."<<endl;
	cout<<"F1 Score is: "<<f1<< "."<<endl;
	fin.close();
    return ;
}
