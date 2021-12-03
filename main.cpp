#include <iostream>
#include <string>
#include "RBtree_for_map.h"
#include "RBtree_for_set.h"
#include "Btree_map.h"
#include "Btree_set.h"
using namespace std;

typedef struct WORD{
    string word;
    char partOfSpeech;
    float frequency;
} Word;


static bool isRBtree = false;


typedef RBmap<string, WORD> RBMAP;
typedef Bmap<string, WORD> BMAP;
typedef Bset<string> BSET;
typedef RBset<string> RBSET;
typedef pair<string,WORD> PAIR;
static RBmap<string, WORD> rbmap;
static Bmap<string, WORD> bmap;
static Bset<string> bset;
static RBset<string> rbset;

void prprpr(Bmap<string, WORD>::Node* x) {
    Bmap<string, WORD>::Node* y = x;
    Bmap<string,WORD>::dataOfValue dov;
    int k = 0;
    if(y!= nullptr){
        k = 0;
        prprpr(y->child[k]);
        for(; k < y->keyNum; k++){
            WORD name = dov^(y->keys[k]);
            cout << name.word <<" "<<name.partOfSpeech<<" "<<name.frequency<< endl;
            prprpr(y->child[k + 1]);
        }
    }
}
void print_Btree(Bmap<string, WORD>& bb){
    if(bb.getROOT() == nullptr)
        return;
    return prprpr(bb.getROOT());
}

void TestRBMap()
{
    RBmap<string, string> m;
    m.insert(std::pair<std::string, std::string>("2222", "2222"));
    m.insert(std::make_pair("1111", "1111"));
    m["0000"] = "0000";
    std::cout <<"size: "<< m.size() << std::endl;
    for (auto e : m)
    {
        cout << e.first <<" "<<e.second << endl;
    }
    m.print_clor_and_key();
    cout << endl;

    RBmap<string, WORD> mm;
    WORD p[2];
    typedef pair<string,WORD> Pair;
    p[0] = {"1111",'N',23.1};
    pair<string,WORD> pair(p[0].word,p[0]);
    mm.insert(::pair<string,WORD>("2",p[0]));
    mm.insert(pair);
    mm["33"] = {"33",'S',11.1};

    for (auto e : mm)
    {
        cout << e.first << " "<<e.second.word <<" "<<e.second.partOfSpeech<<" "<<e.second.frequency << endl;
    }
    std::cout << std::endl;
    m.Delete("0000");
    m.print_clor_and_key();
    p[1] = {"2",'S',33};
    Pair newOne("2",p[1]);
    mm.update("2",newOne);
    for (auto e : mm)
    {
        cout << e.first << " "<<e.second.word <<" "<<e.second.partOfSpeech<<" "<<e.second.frequency << endl;
    }
    mm.find("2");
    WORD find;
    find.word = mm.search("2").second.word;
    find.partOfSpeech = mm.search("2").second.partOfSpeech;
    find.frequency = mm.search("2").second.frequency;
    cout << find.word <<" "<<find.partOfSpeech<<" "<<find.frequency << endl;

}
void TestRBSet()
{
    RBset<string> s;
    s.insert("2222");
    s.insert("1111");
    s.insert("0000");
    cout<<"check is red black tree: "<<((s.check())?"true":"false")<<endl;
    s.insert("9999");
    s.insert("7777");
    std::cout <<"size: "<< s.size() << std::endl;
    cout<<"check is red black tree: "<<((s.check())?"true":"false")<<endl;

    for (auto e : s)
    {
        printf("%s\n", e.c_str());
    }
    s.print();
    std::cout << std::endl;
    s.Delete("7777");
    s.print();
    std::cout << std::endl;
    s.update("9999","ssss");
    s.print();
}
void TestBMap(){
    Bmap<string, string> m;
    m.insert(std::pair<std::string, std::string>("2222", "2222"));
    m.insert(std::make_pair("1111", "1111"));
    std::cout <<"size: "<< m.size() << std::endl;
    //m.print_by_order();
    m.print();
    cout << endl;

    Bmap<string, WORD> mm;
    WORD p[2];

    p[0] = {"1111",'N',23.1};
    pair<string,WORD> pair(p[0].word,p[0]);
    mm.insert(::pair<string,WORD>("2",p[0]));
    mm.insert(pair);
    mm.print(); // print all datas

    std::cout << std::endl;
    m.Delete("0000");
    m.print();
    p[1] = {"2",'S',33};
//    Pair newOne("2",p[1]);
//    mm.update("2",newOne);
    mm.print();
  //  mm.print_by_order();
//    for (auto e : mm)
//    {
//        cout << e.first << " "<<e.second.word <<" "<<e.second.partOfSpeech<<" "<<e.second.frequency << endl;
//    }
    mm.find("2");
    WORD find;
    find.word = mm.search("2").second.word;
    find.partOfSpeech = mm.search("2").second.partOfSpeech;
    find.frequency = mm.search("2").second.frequency;
    cout << find.word <<" "<<find.partOfSpeech<<" "<<find.frequency << endl;
    // cout<<"[]";  可以查询输出
    //cout<<mm["2"].word<<mm["2"].frequency<< endl;
    p[1] = {"5",'s',6};



    Word pppp = {"5",'s',6};
    PAIR p3(pppp.word,pppp);
    BMAP q;
    q.insert(p3);
    prprpr(q.getROOT());

}
void TestBSet(){
    Bset<string> s;
    s.insert("2222");
    s.insert("1111");
    s.insert("0000");
    s.insert("9999");
    s.insert("7777");
    std::cout <<"size: "<< s.size() << std::endl;

//    for (auto e : s)
//    {
//        printf("%s\n", e.c_str());
//    }
    s.print();
    std::cout << std::endl;
    s.Delete("7777");
    s.print();
    std::cout << std::endl;
    s.update("9999","ssss");
    s.print();
}

void search_B_by_key(string cc){
    if(bmap.find(cc) == nullptr){
        //cout << "dont' find"<<endl;
        return ;
    }
    cout << bmap.search(cc).second.word <<" "<<bmap.search(cc).second.partOfSpeech<<" "<<bmap.search(cc).second.frequency<<endl;
}
void search_RB_by_key(string cc){
    if(rbmap.find(cc) == nullptr) {
        //cout << "dont' find" << endl;
        return;
    }
    cout << rbmap.search(cc).second.word <<" "<<rbmap.search(cc).second.partOfSpeech<<" "<<rbmap.search(cc).second.frequency<<endl;
}


void menu() {
    cout << "red-black tree: 1; \nB-tree: 2\ninsert by command: 3\ninsert by file: 4\n"
    <<"delete by command: 5\ndelete by file: 6\n"<<
    "update by key: u\ninitialize: 0\n"<<"search: s\ndump: d\nend: e\nprint menu again: m\nclear: c\nsize: z\n";
}

void set_insert_by_command(bool isRB){
    string command;
    cout<<"input string: "<<endl;
    cin >> command;
    if(isRB){
        rbset.insert(command);
        cout<<"size: "<<rbset.size()<<endl;
        for (auto e : rbset){
            printf("%s\n", e.c_str());
        }
    }
    else {
        bset.insert(command);
        cout<<"size: "<<bset.size()<<endl;
    }
}
void set_delete_by_command(bool isRBtree) {
    string command;
    cout<<"input key: "<<endl;
    cin >> command;
    if(isRBtree){
        rbset.Delete(command);
        for (auto e : rbset)
        {
            printf("%s\n", e.c_str());
        }
    }
    else {
        bset.Delete(command);
        bset.print();
    }
}
void set_update_by_command(bool isRBtree){
    string command;
    cout<<"input string('key' 'new string'):  "<<endl;
    string c1, c2;
    cin >> c1>>c2;
    if(isRBtree) {
        rbset.update(c1, c2);
    }
    else {
        bset.update(c1, c2);
    }
}
void set_search(bool isRBtree){
    string command;
    cout<<"input string('key' 'new string'):  "<<endl;
    cin>>command;
    if(isRBtree){
        cout<<rbset[command]<<endl;
    }
    else {
        //bset.search(command);
        cout<<bset[command]<<endl;
    }
}
void set_dump(bool isRBtree){
    if(isRBtree){
        rbset.print();
    }
    else {
        bset.print();
    }
}
void set_clear(bool isRBtree){
    if(isRBtree){
        rbset.clear();
    }
    else {
        bset.clear();
    }
}
void set_size(bool isRBtree){
    if (isRBtree){
        cout <<"red blue node size: " <<rbset.size()<< endl;
    }
    else {
        cout <<"B tree node size: " <<bset.size()<< endl;
    }
}


void command_insert_by_file(bool isRBT){
    string command;
    cout<<"please input file name: ";
    cin>>command;
    string filename = command;

    ifstream in( filename , ios::in);
    if (!in.is_open()){
        cout <<  "Error: can't open file"  << endl;
        return;
    }
    string line;
    WORD word;
    getline(in,line);

    while (!in.eof()){

        in >> word.word >> word.partOfSpeech >> word.frequency;
        // test word  accuracy
        if(word.frequency < 0 ){
            cout<<"frequency error!"<<endl;
            word.frequency = 0;
        }
        PAIR pair(word.word,word);

        if(isRBT) {
            rbmap.insert(pair);
            //search_RB_by_key(word.word);
        } else {
            bmap.insert(pair);
            //search_B_by_key(word.word);
        }
    }
    in.close();

}
void command_init(){
    string command;
    cout <<"choose red black tree,input 1, or choose B tree, input 2: ";
    cin>>command;
    if(command == "1"){
        isRBtree = true;
    }
    else isRBtree = false;
    command_insert_by_file(isRBtree);
}
void command_insert_by_command(bool isRBT){
    string command;
    cout<<"input 'word' 'partOfSpeech' 'frequency', then press 'enter'"<<endl;
    WORD word;
    cin >> word.word >> word.partOfSpeech >> word.frequency;
    // test word  accuracy
    if(word.frequency < 0 ){
        cout<<"frequency error!"<<endl;
        word.frequency = 0;
    }
    pair<string,WORD> pair(word.word,word);
    cin.ignore(std::numeric_limits<streamsize>::max(), '\n');
    if(isRBT){
        rbmap.insert(pair);
        //search_RB_by_key(word.word);
    } else {
        bmap.insert(pair);
        //search_B_by_key(word.word);
    }
}
void command_delete_by_file(bool isRBT){
    string command;
    cout<<"please input file name: ";
    cin>>command;
    string filename = command;

    ifstream in( filename , ios::in);
    if (!in.is_open()){
        cout <<  "Error: can't open file"  << endl;
        return;
    }
    string line;
    getline(in,line);
    WORD word;

    while (!in.eof()){

        in >> word.word >> word.partOfSpeech >> word.frequency;

        if(isRBT) {
            rbmap.Delete(word.word);
            //search_RB_by_key(word.word);
            //rbmap.print_clor_and_key();
        } else {
            bmap.Delete(word.word);
            //search_B_by_key(word.word);
            //bmap.print_by_order();
        }
    }
    in.close();
}
void command_delete_by_command(bool isRBT) {
    string command;
    cout<<"input 'word', then press 'enter'"<<endl;
    WORD word;
    cin >> word.word ;
    if(isRBT) {
        cout<<"are you sure to delete: (y/n) ";
        search_RB_by_key(word.word);
        cin>>command;
        if(command=="y"){
            rbmap.Delete(word.word);
        }
        else{
            return;
        }
    } else {
        cout<<"are you sure to delete: (y/n) ";
        search_B_by_key(word.word);
        cin>>command;
        if(command=="y"){
            bmap.Delete(word.word);
        }
        else{
            return;
        }
    }
}
void command_update_by_command(bool isRBT){
    string command;
    cout<<"input new 'word' 'partOfSpeech' 'frequency', then press 'enter'"<<endl;
    WORD word;
    cin >> word.word >> word.partOfSpeech >> word.frequency;
    // test word  accuracy
    if(word.frequency < 0 ){
        cout<<"frequency error!"<<endl;
        word.frequency = 0;
    }
    PAIR pair(word.word,word);

    if(isRBT){
        rbmap.update(word.word, pair);
        cout<<"new data in the database is:";
        search_RB_by_key(word.word);
    } else {
        bmap.update(word.word, pair);
        cout<<"new data in the database is:";
        search_B_by_key(word.word);
    }
}
void command_search(bool isRBT){
    string command;
    cout<<"input key(word): ";
    cin>>command;
    if(isRBT){
        search_RB_by_key(command); // print pair
    }
    else {
        search_B_by_key(command);
    }
}
void command_dump(bool isRBT){
    if(isRBT) {
        //rbmap.print_clor_and_key();
        for (auto e : rbmap){
            cout << e.first <<" "<<e.second.partOfSpeech<<" "<<e.second.frequency << endl;
        }
    } else {
        print_Btree(bmap);
    }
}

void command_clear(bool isRBtree){
    if(isRBtree){
        rbmap.clear();
    }
    else{
        bmap.clear();
    }
}
void size(bool isRB){
    if(isRB){
        cout<<" red blue node size: "<<rbmap.size()<<endl;
    }else {
        cout<<"B tree node size: "<<bmap.size()<<endl;
    }
}
void gogoCommand(char command){

    switch (command) {
        case 'm': // print menu
            menu();
            break;
        case '1':
            cout << "you choose Red black tree!" << endl;
            isRBtree = true;
            cout << "please choose mode" << endl;
            break;
        case '2':
            cout << "you choose B tree!" << endl;
            isRBtree = false;
            cout << "please choose mode" << endl;
            break;
        case '3':
            cout << "insert by command" << endl;
            command_insert_by_command(isRBtree);
            break;
        case '4':
            cout << "insert by file" << endl;
            command_insert_by_file(isRBtree);
            break;
        case '5':
            cout << "delete by command" << endl;
            command_delete_by_command(isRBtree);
            break;
        case '6':
            cout << "delete by file" << endl;
           command_delete_by_file(isRBtree);
            break;
        case '0':
            cout << "initialize by file" << endl;
            command_init();
            break;
        case 'u':
            cout << "update" << endl;
            command_update_by_command(isRBtree);
            break;
        case 's':
            cout << "search data by key" << endl;
            command_search(isRBtree);
            break;
        case 'd':
            cout << "dump" << endl;
            command_dump(isRBtree);
            break;
        case 'e':
            exit(1);
        case 'c':
            command_clear(isRBtree);
            break;
        case 'z':
            size(isRBtree);
            break;
        default:
            cout << "command error" << endl;
            break;
    }
}
void gogosetCommand(char c){
    // 解决无效输入
    cin.clear();
    cin.sync();
    switch (c) {
        case 'm': // print menu
            menu();
            break;
        case '1':
            cout << "you choose Red black tree!" << endl;
            isRBtree = true;
            cout << "please choose mode" << endl;
            break;
        case '2':
            cout << "you choose B tree!" << endl;
            isRBtree = false;
            cout << "please choose mode" << endl;
            break;
        case '3':
            cout << "insert by command" << endl;
            set_insert_by_command(isRBtree);
            break;
        case '5':
            cout << "delete by command" << endl;
            set_delete_by_command(isRBtree);
            break;
        case 'u':
            cout << "update" << endl;
            set_update_by_command(isRBtree);
            break;
        case 's':
            cout << "search data by key" << endl;
            set_search(isRBtree);
            break;
        case 'd':
            cout << "dump" << endl;
            set_dump(isRBtree);
            break;
        case 'e':
            exit(1);
        case 'c':
            set_clear(isRBtree);
            break;
        case 'z':
            set_size(isRBtree);
            break;
        default:
            cout << "command error" << endl;
            break;
    }
}






int main() {
    //TestRBMap();
    //TestRBSet();
    //TestBMap();
    //TestBSet();
    menu();
    string command;
    char c = command[0];
    int i = 0;
    cout <<" choose map(multi type), or set(only on type of data): input 1 or 2: "<<endl;
    cin>>command;
    while(1){
        if(i > 0) {  // 解决无效输入
            cin.clear();
            cin.sync();
        }
        i++;
        if(command == "1"){
            cout<<"you choose multi type data!"<<endl;
            while (1) {
                if(i > 0) {  // 解决无效输入
                    cin.clear();
                    cin.sync();
                }
                // 解决无效输入
                cin.clear();
                cin.sync();

                cout << "show your command " << endl;
                cin>>command;
                cin.ignore(std::numeric_limits<streamsize>::max(), '\n');
                if(command == "e"){
                    break;
                }
                else {
                    c = command[0];
                    gogoCommand(c);
                }
                i++;
            }
        }
        else if(command == "2"){
            cout<<"you choose string data!"<<endl;
            while (1) {
                if(i > 0) {  // 解决无效输入
                    cin.clear();
                    cin.sync();
                }
                // 解决无效输入
                cin.clear();
                cin.sync();
                cout << "show your command " << endl;
                cin>>command;
                if(command == "e"){
                    break;
                }
                else {
                    c = command[0];
                    gogosetCommand(c);
                }
                i++;
            }
        }
        else if(command == "e")
            break;
        else{
            cout<< "error input, agian!"<<endl;
        }
    }
    return 0;
}

