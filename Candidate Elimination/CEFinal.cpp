#include<bits/stdc++.h>
using namespace std;
const int MAX=1e9;
int main(){
    ios_base::sync_with_stdio(0);cin.tie(0);cout.tie(0);
	int numberOfAttributes=16;//number of features in the dataset
	vector <int> attributes(numberOfAttributes); //number of attributes for each feature.
	map <int, int> instance[numberOfAttributes+1];
	string names[101];
	map<int,int> mp,mp1;
	mp[0]=1;mp1[1]=0;
	mp[2]=2;mp1[2]=2;
	mp[4]=3;mp1[3]=4;
	mp[5]=4;mp1[4]=5;
	mp[6]=5;mp1[5]=6;
	mp[8]=6;mp1[6]=8;
	for(int i=0;i<numberOfAttributes;i++){
		attributes[i]=2;
	}
	attributes[12]=6;
  	vector <int> currdata(numberOfAttributes+1), tmpData(numberOfAttributes);

  	//set used to represent the generic and specific boundaries.
  	set < vector<int> > specific[8], generic[8];

  	//initialization
  	for(int i=0; i<numberOfAttributes; i++)
  	{
    	tmpData[i]=0;
  	}
  	for(int i=0; i<8; i++)
  	{
    	specific[i].insert(tmpData);
  	}
  	for(int i=0; i<numberOfAttributes; i++)
  	{
    	tmpData[i]=MAX;
  	}
  	for(int i=0; i<8; i++)
  	{
    	generic[i].insert(tmpData);
  	}
  	ifstream infile;
  	infile.open("data.txt");
  	for(int Z=0;Z<101;Z++){
  		string data,temp;
  		getline(infile,data);
  		cout<<data<<"\n";
  		int st = 0, count = 0;
    	int length = data.size();
    	int dummy;
    	for(int i=0;i<length;i++){
    		if(data[i]==','){
    			dummy=i+1;
    			temp=data.substr(st,i-st);
    			names[Z]=temp;
    			break;
    		}
    	}
    	for(int i=dummy;i<length;i+=2){
    		int k=data[i]-'0';
    		currdata[count]=k+1;
    		if(count==12) currdata[count]=mp[k];
    		///cout<<currdata[count]<<' ';
    		count++;
    	}
    	vector<int> m, n, p;
    	for (int x = 1; x < 8; ++x)
    	{
    		if(currdata[count-1]==x+1){
    			set < vector<int> > tempg,temps;
    			tempg=generic[x];
    			for(set<vector<int> >::iterator it=generic[x].begin();it!=generic[x].end();it++){
    				m = *it;
			        int er =0;
			        for(int i=0; i<numberOfAttributes; i++){
			          	if(m[i]!=currdata[i] && m[i]!=MAX ){
			            	er = 1;
			            	break;
			          	}
			        }
			        if(er==1)
			          tempg.erase(m);
    			}
    			generic[x]=tempg;

    			m=*(specific[x].begin());
    			n=*(specific[x].begin());
    			specific[x].erase(n);
    			for(int i=0;i<numberOfAttributes;i++){
    				if(m[i]==0){
    					m[i]=currdata[i];
    				}
    				else if(m[i]!=currdata[i]){
    					m[i]=MAX;
    				}
    			}
    			specific[x].insert(m);

    		}
    		else{
    			set <vector<int> > tempg,temps;
    			n=*(specific[x].begin());
    			for(set<vector<int> >::iterator it=generic[x].begin();it!=generic[x].end();it++){
    				m=*it;
    				int er=0;
    				for(int i=0;i<numberOfAttributes;i++){
    					if(m[i]!=MAX && m[i]!=currdata[i]){
    						er=1;
    						break;
    					}
    				}
    				if(er == 1)
    					tempg.insert(m);
    				else{
    					vector<int> temphyp;
    					for(int i=0;i<numberOfAttributes;i++){
    						if(m[i] == MAX){
    						temphyp=m;
    						for(int j=1;j<=attributes[i];j++){
    							if(j==currdata[i])
    								continue;
    							else{
    								temphyp[i]=j;
    								bool consistent=true;
    								for(int k=0;k<numberOfAttributes;k++){
    									if(temphyp[k]!=MAX && temphyp[k]!=n[k] && n[k]!=0){
    										consistent = false;
    										break;
    									}
    								}
    								if(consistent)
    									tempg.insert(temphyp);
    							}
    						}
    					}
                    }
    				}
    			}
    			generic[x].clear();
    			set <vector<int> > tempgg;

    			for(set<vector<int> >::iterator it=tempg.begin();it!=tempg.end();it++){
    				m=*it;
    				for(set<vector<int> >::iterator jt=tempg.begin();jt!=tempg.end();jt++){
    					if(it==jt)
    						continue;
    					p=*jt;
    					bool consistent=true;
    					for(int k=0;k<numberOfAttributes;k++){
    						if(m[k] != MAX && m[k]!= p[k]){
    							consistent =false;
    							break;
    						}
    					}
    					if(consistent)
    						tempgg.insert(p);
    				}
    			}
    			for(set<vector<int> >::iterator it=tempg.begin();it!=tempg.end();it++){
    				m=*it;
    				if(tempgg.find(m) == tempgg.end())
    					generic[x].insert(m);
    			}
    		}

    	}

  	}
  	infile.close();
    for(int x=1;x<=7;x++){
       vector<int> abc;

  cout<<"specific border"<<endl;
  abc = *(specific[x].begin());
  cout<<"< ";
  for(int i=0; i<numberOfAttributes; i++)
  {
    if( abc[i]==MAX )
      cout<<"?"<<" ";
    else
    {
      if(i!=12) cout<<abc[i]-1<<' ';
      else cout<<mp1[abc[i]]<<' ';
    }
  }
  cout<<">\n";

  cout<<"\ngeneric border"<<endl;
  for(set< vector<int> >::iterator it= generic[x].begin(); it!=generic[x].end(); it++)
  {
    abc = *it;
    cout<<"< ";
    for(int i=0; i< numberOfAttributes; i++)
    {
      if( abc[i]==MAX )
        cout<<"?"<<" ";
      else
      {
        if(i!=12) cout<<abc[i]-1<<' ';
      else cout<<mp1[abc[i]]<<' ';
      }
    }
    cout<<">\n";
  }
  cout<<"*****************************************************\n";
  	}
	return 0;
}
