#include<bits/stdc++.h>

#define bucket_size 3
#define max_trans_amt 500000   
#define max_categ 1500   
#define bits 16

using namespace std;

int no_of_records=60000;
int global_depth=0;
int bucket_index=0;    
int overflow_index=99999;
int start_trans_id=1;     //starting trans id
int rand_records=15;   //no_of records to be considered for hashing
int processing_record=0;

struct record
    {
        unsigned short int trans_id;
        int sale_amt;

        string cus_name;
        int categ;
    };
struct record insert_record;

struct bucket
{
    int local_depth;
    int empty_spaces;
    struct record data[bucket_size];
    int next_ptr;  //overflow pointer
    
};

//Simulated secondary memory
struct bucket sim_sec_mem[100000];



//Bucket address Table(hash map)
map<string,int> buck_add_table;


vector<record> reco; 

string RandomString(int n)   //generates random string
{
    char alpha[26] = { 'a', 'b', 'c', 'd', 'e', 'f', 'g',
                          'h', 'i', 'j', 'k', 'l', 'm', 'n',
                          'o', 'p', 'q', 'r', 's', 't', 'u',
                          'v', 'w', 'x', 'y', 'z' };
      string cname = "";
    for (int i = 0; i < n; i++)
        cname = cname + alpha[rand() % 26];
        
    return cname;
} 



void initialize_bucket_val()
{
    for(int i=0;i<100000;i++)
    {
        sim_sec_mem[i].empty_spaces= bucket_size;
        sim_sec_mem[i].local_depth=0;
        sim_sec_mem[i].next_ptr=-1;
        for(int j=0;j<bucket_size;j++)
        {
            sim_sec_mem[i].data[j].trans_id=0;
           

        }     
     }
}

bool compare_reco(record a, record b)
        {
            return a.sale_amt<b.sale_amt;
        }

void create_dataset()
{
    

    struct record r[no_of_records];
    // assigning random inputs
    srand(time(0));  // seed to rand()
  
     int max=max_categ-1,min=1;
        int range = max - min + 1;


        
    for(int i=0;i<no_of_records;i++)     //creating random records
    {
        r[i].trans_id=start_trans_id++;
        
        r[i].sale_amt=(rand()%max_trans_amt)+1;
        string cname;
        cname=RandomString(3);
        
        r[i].cus_name=cname;

        //calculating normalized random integer for category
        int random = rand()%no_of_records;
      
        if(no_of_records<1500)
            r[i].categ=(rand()%1500)+1;
        else
        {
        r[i].categ=  ((min + random) / (no_of_records/range));
        if(!r[i].categ)
            r[i].categ=1500;
        }

    } 
    shuffle (r,r+no_of_records,default_random_engine(time(0)));
    ofstream fout;
    fout.open("syn_table.txt");
    for(int i=0;i<rand_records;i++)  
    {
        fout<<r[i].trans_id<<"\t"<<r[i].sale_amt<<"\t"<<r[i].cus_name<<"\t"<<r[i].categ;
        if(i!=rand_records-1)
        fout<<"\n";
    }
    fout.close();
}

//compare hash function
int compare_hash(string i,string j,int depth)
{
    string hash_prefix=j.substr(0,depth);
    return i.compare(hash_prefix);
    

}

//Bucket address table expansion
void bucket_add_table_expand()
{
    map<string,int> buck_add_table_duplicate;
    for(auto & i:buck_add_table)
    {
        buck_add_table_duplicate.insert(pair<string,int>(i.first+'0',i.second));
        buck_add_table_duplicate.insert(pair<string,int>(i.first+'1',i.second));
       
    }
    buck_add_table.clear();
  
    buck_add_table.swap(buck_add_table_duplicate);
 
    global_depth++;
}

void local_depth_expand(string bit_string_transid)
{
    int map_bucket_index;
    string map_hash_key,substr_key1,substr_key;
    vector<record> rehash_reco;
    for(auto & i:buck_add_table)
    {
        if(!compare_hash(i.first,bit_string_transid,global_depth))
        {
            map_bucket_index=i.second;
            map_hash_key=i.first;
        }

    }
   
    if(sim_sec_mem[map_bucket_index].empty_spaces==0)
    {
        for(int i=0;i<bucket_size;i++)
        {
            
            insert_record.trans_id=sim_sec_mem[map_bucket_index].data[i].trans_id;
            insert_record.sale_amt=sim_sec_mem[map_bucket_index].data[i].sale_amt;
            insert_record.cus_name=sim_sec_mem[map_bucket_index].data[i].cus_name;
            insert_record.categ=sim_sec_mem[map_bucket_index].data[i].categ;
          
            //Push the records that are to be rehashed in vector 
            rehash_reco.push_back(insert_record);
           
        }

    }
   
    if(global_depth==1)
    {
        sim_sec_mem[bucket_index].local_depth++;
        buck_add_table["0"]=bucket_index++;
        sim_sec_mem[bucket_index].local_depth++;
        buck_add_table["1"]=bucket_index++;
        
       
    }
    else
    {
        // If overflow-> Splitting
       
        substr_key1=map_hash_key.substr(0,sim_sec_mem[map_bucket_index].local_depth);
        int difference=global_depth-sim_sec_mem[map_bucket_index].local_depth;
        int totatupdatte=pow(2,difference-1);
        int count=0;
        for(auto i = buck_add_table.begin();i!=buck_add_table.end();i++)
        {
           
           if(i->second == map_bucket_index){
                if(count<totatupdatte){
                   i->second=bucket_index;
                   sim_sec_mem[i->second].local_depth=sim_sec_mem[map_bucket_index].local_depth+1;
                }
                else{
                 i->second=bucket_index+1;
                    sim_sec_mem[i->second].local_depth=sim_sec_mem[map_bucket_index].local_depth+1;
                }
              count++;
           }

                                                  
        }
        bucket_index+=2;
       
    }
    int rh;
    for( rh=rehash_reco.size()-1;rh>=0;rh--)
    {
        bitset<bits> trans_id_bits(rehash_reco[rh].trans_id);
        string bit_string_transid=trans_id_bits.to_string();
        
        int mapped_bucket_index;
        
        for(auto & i:buck_add_table)
        {
            if(!compare_hash(i.first,bit_string_transid,global_depth))
            {
                mapped_bucket_index=i.second;
                break;

            }

        }

        
        if(sim_sec_mem[mapped_bucket_index].empty_spaces == 0)
        {
            //Overflow Chaining
            
            if(sim_sec_mem[mapped_bucket_index].local_depth==global_depth)
            {
               
                //bucket address table expansion
                bucket_add_table_expand();
            }
            if(sim_sec_mem[mapped_bucket_index].local_depth<global_depth)
            {
                if(sim_sec_mem[mapped_bucket_index].empty_spaces==0)
                {
                    //split the buckets
                    // rehash records
                    local_depth_expand(bit_string_transid);
                }
            }

        }
        else
        {
            sim_sec_mem[mapped_bucket_index].data[bucket_size - sim_sec_mem[mapped_bucket_index].empty_spaces].trans_id=rehash_reco[rh].trans_id;
            sim_sec_mem[mapped_bucket_index].data[bucket_size -sim_sec_mem[mapped_bucket_index].empty_spaces].sale_amt=rehash_reco[rh].sale_amt;
            sim_sec_mem[mapped_bucket_index].data[bucket_size -sim_sec_mem[mapped_bucket_index].empty_spaces].cus_name=rehash_reco[rh].cus_name;
            sim_sec_mem[mapped_bucket_index].data[bucket_size -sim_sec_mem[mapped_bucket_index].empty_spaces].categ=rehash_reco[rh].categ;
        
                
            // cout<<sim_sec_mem[mapped_bucket_index].empty_spaces<<"\t";
            sim_sec_mem[mapped_bucket_index].empty_spaces--;
                   
        }
    }
    if(rh==-1)    
        {
            rehash_reco.clear();
        }
}
void visualise()
{
    cout<<"\nVisualisation"<<"\n";
    for(map<string,int>::iterator it=buck_add_table.begin();it!=buck_add_table.end();++it)
    {
        cout<<" "<<it->first<<" -> "<<it->second<<"\n";
        int index=it->second;

        cout<<"Bucket:\t"<<index<<"\t\t";
        cout<<"LD: "<<sim_sec_mem[index].local_depth<<"\n";
        
        for(int i=0;i<bucket_size;i++)
        {
            if(sim_sec_mem[index].data[i].trans_id!=0)
            {
                cout<<sim_sec_mem[index].data[i].trans_id<<"\t";
                cout<<sim_sec_mem[index].data[i].sale_amt<<"\t";
                cout<<sim_sec_mem[index].data[i].cus_name<<"\t";
                cout<<sim_sec_mem[index].data[i].categ;
                cout<<endl;
            
            }
        
        }
        
       
        cout<<"####################################################\n";
    }
}
void insert_single_reco(struct record single_reco)
{
    ofstream fout;
    fout.open("syn_table.txt");
    fout<<"\n"<<single_reco.trans_id<<"\t"<<single_reco.sale_amt<<"\t"<<single_reco.cus_name<<"\t"<<single_reco.categ;
    fout.close();
    
}


int main()
{
      
    //random dataset generated
    create_dataset();

   
    //initialize data values for all blocks
    initialize_bucket_val();
    
    ifstream fin;
    fin.open("syn_table.txt");
    int a1;
    int a2;
    string a3;
    int a4;
    while(!fin.eof())
    {
        fin>>a1>>a2>>a3>>a4;
        //cout<<a3<<endl;//<<a2<<a3<<a4;
        insert_record.trans_id=a1;
        insert_record.sale_amt=a2;
        insert_record.cus_name=a3;
        insert_record.categ=a4;
        reco.push_back(insert_record);
        //record to be inserted is present in insert_record
        //cout<<insert_record.cus_name;
        
       
        if(global_depth==0)
        {
            if(sim_sec_mem[bucket_index].empty_spaces>0)
            {

                


                sim_sec_mem[bucket_index].data[sim_sec_mem[bucket_index].empty_spaces-1].trans_id=insert_record.trans_id;
                sim_sec_mem[bucket_index].data[sim_sec_mem[bucket_index].empty_spaces-1].sale_amt=insert_record.sale_amt;
                sim_sec_mem[bucket_index].data[sim_sec_mem[bucket_index].empty_spaces-1].cus_name=insert_record.cus_name;
                sim_sec_mem[bucket_index].data[sim_sec_mem[bucket_index].empty_spaces-1].categ=insert_record.categ;
                 
                 processing_record++;
                sim_sec_mem[bucket_index].empty_spaces--;
               
 
            }
            else if(reco.size()>bucket_size){
                 if(sim_sec_mem[bucket_index].empty_spaces==0)
                {
                    bucket_index++;
                }
                 global_depth++;
              
                //global_depth=local_depth=0
                //bucket address table expansion  
                //directory split
                buck_add_table["0"]=bucket_index-1;
                buck_add_table["1"]=bucket_index-1;
                sim_sec_mem[bucket_index-1].local_depth=0;
               
                
            }
          
            

        }

    }
    fin.close();
  
    

  
    for(int p=processing_record;p<reco.size();p++)
    {

        bitset<bits> trans_id_bits(reco[p].trans_id);
        string bit_string_transid=trans_id_bits.to_string();
      
        int mapped_bucket_index;
        for(auto & i:buck_add_table)
        {
            if(!compare_hash(i.first,bit_string_transid,global_depth))
                mapped_bucket_index=i.second;

        }
        
        if(sim_sec_mem[mapped_bucket_index].empty_spaces == 0)
        {
            if(sim_sec_mem[mapped_bucket_index].local_depth==global_depth)
            {
                
                //bucket address table expansion
                bucket_add_table_expand();
            }
            if(sim_sec_mem[mapped_bucket_index].local_depth<global_depth)
            {
                if(sim_sec_mem[mapped_bucket_index].empty_spaces==0)
                {
                    //split the buckets
                    // rehash records
                    local_depth_expand(bit_string_transid);

                
                   
                    for(auto & i:buck_add_table)
                    {
                        if(!compare_hash(i.first,bit_string_transid,global_depth))
                            mapped_bucket_index=i.second;

                    }
                    if(sim_sec_mem[mapped_bucket_index].empty_spaces==0)
                    {
                        
                        // Chaining
                        // int t_index=mapped_bucket_index;
                        // while(sim_sec_mem[t_index].next_ptr!=-1)
                        // {
                        //     t_index=sim_sec_mem[t_index].next_ptr;
                        // }
                        // sim_sec_mem[t_index].next_ptr=overflow_index;
                        
                        // sim_sec_mem[mapped_bucket_index].data[bucket_size - sim_sec_mem[mapped_bucket_index].empty_spaces].trans_id=reco[p].trans_id;
                        // sim_sec_mem[mapped_bucket_index].data[bucket_size - sim_sec_mem[mapped_bucket_index].empty_spaces].sale_amt=reco[p].sale_amt;
                        // sim_sec_mem[mapped_bucket_index].data[bucket_size - sim_sec_mem[mapped_bucket_index].empty_spaces].cus_name=reco[p].cus_name;
                        // sim_sec_mem[mapped_bucket_index].data[bucket_size - sim_sec_mem[mapped_bucket_index].empty_spaces].categ=reco[p].categ;
                    
                       
                        // sim_sec_mem[overflow_index].empty_spaces--;


                        // overflow_index--;
                        
                    }
                    else
                    {   
                        sim_sec_mem[mapped_bucket_index].data[bucket_size - sim_sec_mem[mapped_bucket_index].empty_spaces].trans_id=reco[p].trans_id;
                        sim_sec_mem[mapped_bucket_index].data[bucket_size - sim_sec_mem[mapped_bucket_index].empty_spaces].sale_amt=reco[p].sale_amt;
                        sim_sec_mem[mapped_bucket_index].data[bucket_size - sim_sec_mem[mapped_bucket_index].empty_spaces].cus_name=reco[p].cus_name;
                        sim_sec_mem[mapped_bucket_index].data[bucket_size - sim_sec_mem[mapped_bucket_index].empty_spaces].categ=reco[p].categ;
                    
                           
                        sim_sec_mem[mapped_bucket_index].empty_spaces--;

                        
                    }
                }
                else
                {
                     sim_sec_mem[mapped_bucket_index].data[bucket_size - sim_sec_mem[mapped_bucket_index].empty_spaces].trans_id=reco[p].trans_id;
                        sim_sec_mem[mapped_bucket_index].data[bucket_size - sim_sec_mem[mapped_bucket_index].empty_spaces].sale_amt=reco[p].sale_amt;
                        sim_sec_mem[mapped_bucket_index].data[bucket_size - sim_sec_mem[mapped_bucket_index].empty_spaces].cus_name=reco[p].cus_name;
                        sim_sec_mem[mapped_bucket_index].data[bucket_size - sim_sec_mem[mapped_bucket_index].empty_spaces].categ=reco[p].categ;
                    
                       
                    sim_sec_mem[mapped_bucket_index].empty_spaces--;
                    
                }
            }
        }
        else
        {
            sim_sec_mem[mapped_bucket_index].data[bucket_size - sim_sec_mem[mapped_bucket_index].empty_spaces].trans_id=reco[p].trans_id;
            sim_sec_mem[mapped_bucket_index].data[bucket_size - sim_sec_mem[mapped_bucket_index].empty_spaces].sale_amt=reco[p].sale_amt;
            sim_sec_mem[mapped_bucket_index].data[bucket_size - sim_sec_mem[mapped_bucket_index].empty_spaces].cus_name=reco[p].cus_name;
            sim_sec_mem[mapped_bucket_index].data[bucket_size - sim_sec_mem[mapped_bucket_index].empty_spaces].categ=reco[p].categ;
                    
            sim_sec_mem[mapped_bucket_index].empty_spaces--;
            
        }
        // visualise();
    }



    //print bucket address table
     for(map<string,int>::iterator it=buck_add_table.begin();it!=buck_add_table.end();++it)
        {
            cout<<" "<<it->first<<" -> "<<it->second<<"\n";
        }
        cout<<"\n";
    visualise();
    // int x=1;
    // if(x)
    // {
    //     insert_record.trans_id=1;
    //     insert_record.sale_amt=1234;
    //     insert_record.cus_name="abc";
    //     insert_record.categ=1;
    //     insert(insert_record);
    //     x--;
    // }

    return 0;
}
