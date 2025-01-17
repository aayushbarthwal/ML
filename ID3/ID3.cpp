using namespace std;

#include <bits/stdc++.h>
#define ff first
#define ss second
#define pb push_back
int ct = 0,bt = 0;
char tempstore[300];
vector<vector<string> >trainingdata,testingdata;
vector<set<string> >dataset(15),testdataset(15);  ///contain all possible data of each attribute
int numberOfAttributes = 15;

int containsMissingData[1000005];
struct attribute;
struct internalnode;

struct attribute{
    int attribute_num; // Attribute number
    int answer;
    bool leaf;  //to check whether its a leaf or not
    vector<internalnode *>next;   //pointers to all internal nodes
};


struct internalnode{
    int node_num;
    attribute *nxt;   //internal nodes
};

vector<string>threshold[15];
double entropy(vector<vector<string> > data){    //entropy calculation
    int pos, neg, total;
    pos = neg = total = 0;

    int len = data.size()-1;

    for(int i=0 ; i<data.size() ; i++){
        vector<string>vec = data[i];
        if(vec[14] == ">50K")
            pos++;
        else 
            neg++;
    }
  
    total = pos + neg;  
    if(pos == 0 || neg == 0)
        return 0.0;

    int x = pos, y = neg;
    
    double p1, p2;
    p1 = (1.0 * pos) / total, p2 = (1.0 * neg) / total;
    
    return -1.0 * (p1 * log2(p1) + p2 * log2(p2));
}

double entropy(int greater, int lesser){
    int total = greater + lesser;
    
   if(greater == 0 || lesser == 0)
        return 0.0;
    
    double p1, p2;
    p1 = (1.0 * greater) / total, p2 = (1.0 * lesser) / total;
    
    return -1.0 * (p1 * log2(p1) + p2 * log2(p2));
}

int inf_gain(vector<vector<string> > data, set<int> used){    //this function returns index with maximum information gain
    bool flag = false;
    int index = -1;
    double val = 0.0, entrpy = 0.0, tempEntropy;

    entrpy = entropy(data);
 
    if(entrpy == 0)
        return index;
    
  for(int attribute = 0 ; attribute < numberOfAttributes - 1; attribute++){
        if(used.find(attribute) == used.end()){
            int len = dataset[attribute].size();
          
            int cnt[len][2], total = 0;     //cnt[][] stores number of data with >"50K" and "<=50K" values 
            
            for(int j=0 ; j<len ; j++)
                cnt[j][0] = cnt[j][1] = 0;

            int j = 0;

            for(set<string>::iterator it = dataset[attribute].begin();it!=dataset[attribute].end();it++){
                for(int k=0 ; k<data.size() ; k++){
                    if(data[k][attribute] == *it){
                        total++;
                        if(data[k][14] == ">50K"){
                            cnt[j][1]++;
                        } else {
                            cnt[j][0]++;
                        }
                    }
                }
                j++;
            }

            if(total == 0)
                continue;
            
            tempEntropy = entrpy;
            
            for(int j=0 ; j<len ; j++){
                tempEntropy -= ((cnt[j][0] * 1.0 + cnt[j][1]) / total) * entropy(cnt[j][0], cnt[j][1]);
            }
           
            
           if(flag){
                if(tempEntropy > val){
                    val = tempEntropy;
                    index = attribute;
                }
           } else {
               val = tempEntropy;
                flag = true;
               index = attribute;
            }
        }
    }    
    return index;
}


void ID3(attribute **root,vector<vector<string> >data,set<int>used){
    
    int att_used = inf_gain(data,used);   //att_used is node with maximum inf_gain and not used yet

    *root = new attribute();
    (*root)->leaf =false;

    (*root)->attribute_num = att_used;
    int output[2] = {0};

     if(att_used == -1){
        (*root)->leaf = true;
        
    }

    for(int i = 0; i<data.size();i++){
       if(data[i][14] == ">50K")output[1]++;
       else
        output[0]++;

    }
    
if(output[1]>=output[0])
    (*root)->answer = 1;
else
    (*root)->answer = 0;



      if(att_used == -1)
        return;

    *root = new attribute();
    (*root)->leaf =false;
    (*root)->attribute_num = att_used;    
    int len = dataset[att_used].size();

    vector<vector<vector<string> > >tempdataused(len);
    int j = 0;

    for(set<string>::iterator it = dataset[att_used].begin();it!=dataset[att_used].end();it++){
            string str = *it;
        
            for(int i = 0; i<data.size();i++){
                if(data[i][att_used] == str)
                    tempdataused[j].pb(data[i]);
            }
            j++;
    }

    used.insert(att_used);   //used set where node already used is stored


    for(int i=0 ; i<len ; i++){
        internalnode *in = (internalnode *)malloc(sizeof(internalnode));
        in->node_num = i;      
        in->nxt = NULL;
        (*root)->next.pb(in);        
    }

    for(int i=0 ; i<len ; i++){
        if(tempdataused[i].size()){
          ID3(&((*root)->next[i]->nxt),tempdataused[i], used);
        }
   }
}


int search(attribute **root,vector<string>test){
     if(*root == NULL)
       return 1;

    if((*root)->leaf == true){

        int temp = (*root)->answer;
        return temp;

    }

    int index = (*root)->attribute_num;
    string str = test[index];
    int j = 0;
  
  
      for(set<string>::iterator i = dataset[index].begin(); i!=dataset[index].end();i++){
            if(*i == str){ break;}
            j++;
    }
 
    return search(&((*root)->next[j])->nxt,test);
    
}




int main(int argc, char const *argv[])
{
     int flag = 0;
    FILE *inputfile = fopen("adult.txt","r+");   //trainingdata file


    while(fscanf(inputfile, "%[^\n]s",tempstore)!=EOF){  //storing training data in training data vector
        int len = strlen(tempstore),j = 0;
        vector<string>strtemp;
      
        while(tempstore[j] == ' ')j++;
        int p = 0;
        string temp = "";
        for(;j<len;j++){
            if(tempstore[j] ==' ' || tempstore[j] ==','){
                if(temp!=""){
                strtemp.pb(temp);
                threshold[p].pb(temp);
                p++;
                
                }
                temp = "";
                continue;
            }
            temp.pb(tempstore[j]);
            
        }
        strtemp.pb(temp);
        trainingdata.pb(strtemp);

        int lastinserted = trainingdata.size()-1;
      
        for(int i = 0; i<trainingdata[lastinserted].size();i++){
         string str = trainingdata[lastinserted][i];
         if(str!="?")
          dataset[i].insert(str);    
        }
       fgetc(inputfile);

    }

     for(int i = 0; i<15; i++){     //giving values to missing variables with discrete values
                 if( i == 0 || i == 2 || i == 4 || i == 10 || i == 11 || i == 12)continue;
                int len = dataset[i].size();
                int ct[len];
            for(set<string>::iterator it = dataset[i].begin();it!=dataset[i].end();it++){
                
                int pos = distance(dataset[i].begin(),it);//(it - dataset[i].begin());
          
                    for(int t = 0; t<len ;t++)ct[t] = 0;
                        for(int j = 0; j<trainingdata.size();j++){
                            if(trainingdata[j][i] == *it)
                            ct[pos]++;
                        }
                }

                        int index = -1,maxvalue = -1;
                            for(int t = 0; t<len;t++){
                                    if(ct[t]>maxvalue){
                                        maxvalue = ct[t];
                                        index = t;
                                    }
                            }

                    string copy_string = "";
                    int val = 0;
                    for(set<string>::iterator it = dataset[i].begin();it!=dataset[i].end();it++){
                            if(val == index){
                                copy_string = *it;
                                break;
                            }
                            val++;
                    }
                  

                   for(int t = 0 ; t<trainingdata.size();t++)

                        if(trainingdata[t].size() == 15 && trainingdata[t][i] == "?" ){
                         
                            trainingdata[t][i] =""; //copy_string;
                            trainingdata[t][i] +=copy_string;  
                        }
        
    }
        
    int arr[15] = {0};

    for(int i = 0; i<trainingdata.size();i++){  //giving values to missing data with continous values

        for(int t = 0; t<15; t++)
            if( t == 0 || t == 2 || t == 4 || t == 10 || t == 11 || t == 12){
                string str = trainingdata[i][t];
                if(str == "?")continue;
                char tempstring[100];

                for(int pp = 0; pp <str.size();pp++)
                    tempstring[pp] = str[pp];
                    int x = atoi(tempstring);
                    arr[t]+=x;
            }

    }

    int trainingdatasize = trainingdata.size();

    for(int i = 0; i<trainingdatasize;i++){
        for(int t = 0; t<trainingdata[i].size();i++)
          if( t == 0 || t == 2 || t == 4 || t == 10 || t == 11 || t == 12){
               
            if(trainingdata[i][t] == "?"){ 
                int temp = (arr[t]/trainingdatasize);
                stringstream ss;
                ss<<temp;
                trainingdata[i][t] =ss.str();
          }
    }
    }

    
   for(int i = 0; i<15; i++)
        sort(threshold[i].begin(),threshold[i].end());


    for(int i = 0; i<15; i++){    //storing dataset  i.e possible  values of continous values

        if( i ==0 || i == 2 || i == 4 || i == 10 || i == 11|| i == 12){
            dataset[i].clear();
            dataset[i].insert("0");
            dataset[i].insert("1");
        }
        
    }   
 
    dataset[0].insert("2");
    dataset[0].insert("3");

            for(int j = 0; j<trainingdata.size();j++){     // spiltting training data with continous values
                for(int t = 0; t<trainingdata[j].size();t++){

                        if(t == 0){
                            if(trainingdata[j][t]<="30")trainingdata[j][t] = "0";
                            else if(trainingdata[j][t]<="45")trainingdata[j][t] = "1";
                            else if(trainingdata[j][t]<="60")trainingdata[j][t] = "2";
                            else if(trainingdata[j][t]<="90")trainingdata[j][t] = "3";
                        }
                        if(t == 2){
                            if(trainingdata[j][t]<="60000")trainingdata[j][t] = "0";
                            else
                                trainingdata[j][t] = "1";
                        }
                         if(t == 4){
                            if(trainingdata[j][t]<="8")trainingdata[j][t] = "0";
                            else
                                trainingdata[j][t] = "1";
                        }

                         if(t == 10){
                            if(trainingdata[j][t]<="10000")trainingdata[j][t] = "0";
                            else
                                trainingdata[j][t] = "1";
                        }
                         if(t == 11){
                            if(trainingdata[j][t]<="2000")trainingdata[j][t] = "0";
                            else
                                trainingdata[j][t] = "1";
                        }
                         if(t == 12){
                            if(trainingdata[j][t]<="47")trainingdata[j][t] = "0";
                            else
                                trainingdata[j][t] = "1";
                        }
                    }

                
            }
    attribute *root;
    set<int>temp;

   ID3(&root,trainingdata,temp);    //tree creation

    FILE *testingfile = fopen("test.txt","r+");                     //testing data reading
    while(fscanf(testingfile, "%[^\n]s",tempstore)!=EOF){
        int len = strlen(tempstore),j = 0;
        vector<string>strtemp;
    
        while(tempstore[j] == ' ')j++;
        int p = 0;
        string temp = "";
        for(;j<len;j++){
            if(tempstore[j] ==' ' || tempstore[j] ==','){
                if(temp!=""){
                    strtemp.pb(temp);  
                    p++;   
                }
                temp = "";
                continue;
            }
            temp.pb(tempstore[j]);
            
        }
        strtemp.pb(temp);
        testingdata.pb(strtemp);

        int lastinserted = testingdata.size()-1;
      
        for(int i = 0; i<testingdata[lastinserted].size();i++){
         string str = testingdata[lastinserted][i];
         if(str!="?")
          testdataset[i].insert(str);    
        }

     
       fgetc(testingfile);

    }

     for(int i = 0; i<15; i++){                 //giving values to missing testing data with discrete values
                if( i == 0 || i == 2 || i == 4 || i == 10 || i == 11 || i == 12)continue;
                int len = dataset[i].size();
                int ct[len];
            for(set<string>::iterator it = dataset[i].begin();it!=dataset[i].end();it++){
               
                int pos = distance(dataset[i].begin(),it);
        
                    for(int t = 0; t<len ;t++)ct[t] = 0;
                       for(int j = 0; j<testingdata.size();j++){
                            if(testingdata[j].size() == 15 && testingdata[j][i] == *it)
                            ct[pos]++;
                        }
                }
            

                       int index = -1,maxvalue = -1;
                            for(int t = 0; t<len;t++){
                                    if(ct[t]>maxvalue){
                                        maxvalue = ct[t];
                                        index = t;
                                    }
                            }

                    string copy_string = "";
                    int val = 0;
                    for(set<string>::iterator it = dataset[i].begin();it!=dataset[i].end();it++){
                            if(val == index){
                                copy_string = *it;
                                break;
                            }
                            val++;
                    }
                   

                 for(int t = 0 ; t<testingdata.size();t++)

                        if(testingdata[t].size() == 15 && testingdata[t][i] == "?" ){
                          
                            testingdata[t][i] = "";
                            testingdata[t][i]+= copy_string;
                          
                        }
            
    }
    memset(arr,0,sizeof(arr));
    for(int i = 0; i<testingdata.size();i++){             //giving values to missing testing data with continous values
        for(int t = 0; t<15; t++)
            if( (testingdata[i].size() == 15) && (t == 0 || t == 2 || t == 4 || t == 10 || t == 11 || t == 12)){
                string str = testingdata[i][t];
                if(str == "?")continue;
                char tempstring[100];
                for(int pp = 0; pp <str.size();pp++)
                    tempstring[pp] = str[pp];
                    int x = atoi(tempstring);
                    arr[t]+=x;
            }
    }
    int testingdatasize = testingdata.size();

    for(int i = 0; i<testingdatasize;i++){                
        for(int t = 0; t<testingdata[i].size();i++)
           if( (testingdata[t].size() == 15) && (t == 0 || t == 2 || t == 4 || t == 10 || t == 11 || t == 12)){
               
            if(testingdata[i][t] == "?"){ 
                int temp = (arr[t]/testingdatasize);
                stringstream ss;
                ss<<temp;
                trainingdata[i][t] =ss.str();
          }
    }
    }

     for(int i = 0; i<15; i++){

        if( i ==0 || i == 2 || i == 4 || i == 10 || i == 11|| i == 12){
            testdataset[i].clear();
            testdataset[i].insert("0");
            testdataset[i].insert("1");
        }
        
    }   
 
    testdataset[0].insert("2");
    testdataset[0].insert("3");


    for(int j = 0; j<testingdata.size();j++){
                for(int t = 0; t<testingdata[j].size();t++){ //branching continous values
                   if(testingdata[j].size()<15)continue;
                      

                      if( t ==0 || t == 2 || t == 4 ||t == 10 || t == 11 || t == 12 ){
                        int mid = threshold[t].size()/2;
                            if(testingdata[j][t]<=threshold[t][mid])testingdata[j][t] = "0";
                            else
                                testingdata[j][t] = "1";
                        }
                    }
                    

                    
           }     
                
            
       
   
int  total = 0;
int ans = 0,inx;
int tp = 0,tn = 0, fn=0, fp=0 ;
int wrong = 0,left = 0;
for(int i = 0; i<testingdata.size();i++){           //TESTING 


if(testingdata[i].size() == 15){
        ans = search(&root,testingdata[i]);
        total++;

        if(  ans == 1 && testingdata[i][14] == ">50K." ){tp++;
               cout<<">50K"<<"------------------"<<">50K"<<endl;
        }
        else if(ans == 0 && testingdata[i][14] == "<=50K."){
         tn++;
        cout<<"<=50K"<<"----------------"<<"<=50K"<<endl;
        }
        
        else if( ans == 0 && testingdata[i][14] == ">50K."){fn++;
         cout<<"<=50K"<<"xxxxxxxxxxxxxxxxxx"<<">50K"<<"   "<<endl;
        }

         else if( ans == 1 && testingdata[i][14] == "<=50K."){fp++;
         cout<<">50K"<<"xxxxxxxxxxxxxxxxxx"<<"<=50K"<<"    "<<endl;
        }
        
}
}
double acc, pre, rec, fscore;
acc= 1.0*(tp+tn)/(total);
pre= 1.0*(tp)/(fp+tp);
rec= 1.0*(tp)/(fn+tp);
fscore= 2* (pre*rec)/(pre+rec);
cout<<endl;
cout<<"Correctly Predicted:"<<tp+tn<<endl;
cout<<"Total Cases:        "<<total<<endl;
cout<<"----------------------------"<<endl;

cout<<"Accuracy is:         "<<acc*100<<"%"<<endl;   
cout<<"Precision is:        "<<pre*100<<"%"<<endl;   
cout<<"Recall is:           "<<rec*100<<"%"<<endl;   
cout<<"Fscore is:           "<<fscore<<endl;     
    return 0;

}
