#include <bits/stdc++.h>
using namespace std;
double pos=0, neg=0, binpos=0, binneg=0, spos=0, sneg=0, sbinpos=0, sbinneg=0;         //declaring variables to store each occurance
double check1=log(0.5), check2=log(0.5), check3=log(0.5), check4=log(0.5), check5=log(0.5), check6=log(0.5), check7=log(0.5), check8=log(0.5);   //declaring variables to store probabilities
int tp=0, fn=0, fp=0, tn=0, btp=0, bfn=0, bfp=0, btn=0, stp=0, sfn=0, sfp=0, stn=0, sbtp=0, sbfn=0, sbfp=0, sbtn=0;  //variables for each case
int words[100000][4]={0};    //array to store words
int stopwords[100000][4]={0};
int vocab= 89527;         //total number of words
int svocab= vocab-173;

double checkpos(int x)       //checking probability of positive
{
	double a= 1 + words[x][0];
	double b= pos+vocab;
	return log(a/b);
}

double checkneg(int x)       //checking probability of negative
{
	double a= 1 + words[x][1];
	double b= neg+vocab;
	return log(a/b);
}

double checkbinpos(int x)	//checking probability of binary positive
{
	double a= 1 + words[x][2];
	double b= binpos+vocab;
	return log(a/b);
}
double checkbinneg(int x)	//checking probalility of binary negative
{
	double a= 1 + words[x][3];
	double b= binneg+vocab;
	return log(a/b);
}

double checkspos(int x)       //checking probability of positive
{
	double a= 1 + stopwords[x][0];
	double b= spos+svocab;
	return log(a/b);
}

double checksneg(int x)       //checking probability of negative
{
	double a= 1 + stopwords[x][1];
	double b= sneg+svocab;
	return log(a/b);
}

double checksbinpos(int x)	//checking probability of binary positive
{
	double a= 1 + stopwords[x][2];
	double b= sbinpos+svocab;
	return log(a/b);
}
double checksbinneg(int x)	//checking probalility of binary negative
{
	double a= 1 + stopwords[x][3];
	double b= sbinneg+svocab;
	return log(a/b);
}


int main(){

ifstream fin("train.feat",ios::in); 		//reading the training file

while(!fin.eof())
{
	string str;
	fin>>str;
	int r=atoi(str.c_str());
	while(fin>>str)
	{
        int st=0,en=0;
        int x[2],k=0;

        while(en<str.size())
        {
            for(;str[en]!=':' && str[en]!='\0' && str[en]!='\n';en++);
            string s=str.substr(st,en-st);
            x[k++] = atoi(s.c_str());
            if(str[en]=='\0')  break;
            en++;
            st=en;
            
        }


		if(r>5)
		{
            words[x[0]][0] = words[x[0]][0]+x[1];    //storing the words in the provided column
            words[x[0]][2] = words[x[0]][2]+1;
		}
		else 
		{
            words[x[0]][1] = words[x[0]][1]+x[1];
            words[x[0]][3] = words[x[0]][3]+1;
		}
		if(fin.peek() == '\n') 
            break;
	}
}

fin.close();

int i,j;
for(i=0;i<100000;i++){								//calculating column frequency							
	pos=pos+words[i][0];                          
	neg=neg+words[i][1];
	binpos=binpos+words[i][2];
	binneg=binneg+words[i][3];
}
vector<string> vocab;															//removing stopwords
	int stoplist[174];												
	int il=0;
	vector<string>::iterator it;
	ifstream fin3("imdb.vocab",ios::in);										//reading the vocab file		
	string line;	
	while(fin3>>line){
		vocab.push_back(line);
	}
	ifstream fin4("stopwords.txt",ios::in);										//reading the stopwords
	string stop;
	while(fin4>>stop){
	it = find(vocab.begin(),vocab.end(),stop);
	int position = distance(vocab.begin(),it);									//finding the index of the stopword
	
	stoplist[il]=position;														//storing the index in an array
	il++;
}

for(i=0;i<4;i++)
	{for(int j=0;j<100000;j++)
	  {stopwords[j][i]=words[j][i];
	  }
	}
for(i=0;i<174;i++){
	for(j=0;j<4;j++)
	{stopwords[stoplist[i]][j]=0;
	}
}
for(i=0;i<100000;i++){								//calculating column frequency							
	spos=spos+stopwords[i][0];                          
	sneg=sneg+stopwords[i][1];
	sbinpos=sbinpos+stopwords[i][2];
	sbinneg=sbinneg+stopwords[i][3];
}


ifstream fin2("test.feat",ios::in);					//reading the test file

while(!fin2.eof())
{
	string str;
	fin2>>str;
	int r=atoi(str.c_str());
	//cout<<r<<endl;
	while(fin2>>str)
	{
        int st=0,en=0;
        int x[2],k=0;
        while(en<str.size())
        {
            for(;str[en]!=':' && str[en]!='\0' && str[en]!='\n';en++);
            string s=str.substr(st,en-st);
            x[k++] = atoi(s.c_str());
            if(str[en]=='\0')  break;
            en++;
            st=en;
            check1=check1 + 1.0*x[1]*checkpos(x[0]);                     //calculating probabilities for each class
			check2=check2 + 1.0*x[1]*checkneg(x[0]);
			check3=check3 + 1.0*checkbinpos(x[0]);
			check4=check4 + 1.0*checkbinneg(x[0]);
			check5=check5 + 1.0*x[1]*checkspos(x[0]);
			check6=check6 + 1.0*x[1]*checksneg(x[0]);
			check7=check7 + 1.0*checksbinpos(x[0]);
			check8=check8 + 1.0*checksbinneg(x[0]);
        }
        //cout<<x[0]<<":"<<x[1]<<" ";

		
         if(fin2.peek() == '\n') //detect "\n"
        {
         // cout<<endl;
            break;
        }
	} 
	if(r>5){															//classifying the dataset
		if(check1>check2)
			tp++;
		else
			fn++;
		if(check5>check6)
			stp++;
		else
			sfn++;
		 
		}								
	
	else{
		if(check1>check2)
			fp++;
		else
			tn++;
		if(check5>check6)
			sfp++;
		else
			stn++;	
		}
	if(r>5){
		if(check3>check4)
			btp++;
		else 
			bfn++;
		if(check7>check8)
			sbtp++;
		else
			sbfn++;
		}								
	
	else{
		if(check3>check4)
			bfp++;
		else
			btn++;
		if(check7>check8)
			sbfp++;
		else 
			sbtn++;
		}

}
fin2.close();
int stopwords[100000][4];
for(i=0;i<4;i++)
	{for(int j=0;j<100000;j++)
	  {stopwords[j][i]=words[j][i];
	  }
	}
for(i=0;i<174;i++){
	for(j=0;j<4;j++)
	{stopwords[stoplist[i]][j]=0;
	}
}

/*int total=tp+fn+fp+tn;
cout<<tp<<" "<<tn<< " "<<fp<< " "<<fn<<endl;
cout<<total<<endl;
int totalb=btp+bfn+bfp+btn;
int stotal=stp+sfn+sfp+stn;
int stotalb=sbtp+sbfn+sbfp+sbtn;*/
double acc= 1.0*(tp+tn)/25000;
double pre= 1.0*(tp)/(fp+tp);
double npre= 1.0*(tn)/(fn+tn);
double rec= 1.0*(tp)/(fn+tp);
double nrec= 1.0*(tn)/(fp+tn);
double fscore= 2* (pre*rec)/(pre+rec);
double nfscore= 2* (npre*nrec)/(npre+nrec);
double bacc= 1.0*(btp+btn)/25000;
double bpre= 1.0*(btp)/(bfp+btp);
double nbpre= 1.0*(btn)/(bfn+btn);
double brec= 1.0*(btp)/(bfn+btp);
double nbrec= 1.0*(btn)/(bfp+btn);
double bfscore= 2* (bpre*brec)/(bpre+brec);
double nbfscore= 2* (nbpre*nbrec)/(nbpre* nbrec);
double sacc= 1.0*(stp+stn)/25000;
double spre= 1.0*(stp)/(sfp+stp);
double nspre= 1.0*(stn)/(sfn+stn);
double srec= 1.0*(stp)/(sfn+stp);
double nsrec= 1.0*(stn)/(sfp+stn);
double sfscore= 2* (spre*srec)/(spre+srec);
double nsfscore= 2* (nspre*nsrec)/(nspre+nsrec);
double sbacc= 1.0*(sbtp+sbtn)/25000;
double sbpre= 1.0*(sbtp)/(sbfp+sbtp);
double nsbpre= 1.0*(sbtn)/(sbfn+sbtn);
double sbrec= 1.0*(sbtp)/(sbfn+sbtp);
double nsbrec= 1.0*(sbtn)/(sbfp+sbtn);
double sbfscore= 2* (sbpre*sbrec)/(sbpre+sbrec);
double nsbfscore= 2* (nsbpre*nsbrec)/(nsbpre+nsbrec);
cout<<"-------------------------------With stopwords----------------------------"<<endl;
cout<<"For positive sentiment                            For negative sentiment"<<endl;
cout<<"----------------------                            ----------------------"<<endl;
cout<<"Accuracy is:  "<<acc<<"                             "<<"Accuracy is:  "<<acc<<endl;
cout<<"Precision is: "<<pre<<"                            "<<"Precision is: "<<npre<<endl;
cout<<"Recall is:    "<<rec<<"                             "<<"Recall is:    "<<nrec<<endl;
cout<<"Fscore is:    "<<fscore<<"                            "<<"Fscore is:    "<<nfscore<<endl;
cout<<"                  **********with binarization**********"<<endl;
cout<<"Accuracy is:  "<<bacc<<"                             "<<"Accuracy is:  "<<bacc<<endl;
cout<<"Precision is: "<<bpre<<"                            "<<"Precision is: "<<nbpre<<endl;
cout<<"Recall is:    "<<brec<<"                                   "<<"Recall is:    "<<nbrec<<endl;
cout<<"Fscore is:    "<<bfscore<<"                            "<<"Fscore is:    "<<nbfscore<<endl;
cout<<"-----------------------------Without stopwords---------------------------"<<endl;
cout<<"Accuracy is:  "<<sacc<<"                             "<<"Accuracy is:  "<<sacc<<endl;
cout<<"Precision is: "<<spre<<"                            "<<"Precision is: "<<nspre<<endl;
cout<<"Recall is:    "<<srec<<"                              "<<"Recall is:    "<<nsrec<<endl;
cout<<"Fscore is:    "<<sfscore<<"                            "<<"Fscore is:    "<<nsfscore<<endl;
cout<<"                  **********with binarization**********"<<endl;
cout<<"Accuracy is:  "<<sbacc<<"                             "<<"Accuracy is:  "<<sbacc<<endl;
cout<<"Precision is: "<<sbpre<<"                             "<<"Precision is: "<<nsbpre<<endl;
cout<<"Recall is:    "<<sbrec<<"                                   "<<"Recall is:    "<<nsbrec<<endl;
cout<<"Fscore is:    "<<sbfscore<<"                            "<<"Fscore is:    "<<nsbfscore<<endl;
return 0;
}



