/*
* [32204292 모바일시스템공학과 조민혁]
* Mobile processor Programming assignment #1: Simple calculator
* 프로젝트 시작일 : 2024/03/07
* 프로젝트 종료일 : 2024/03/31
* 구현 범위 : 사칙연산, 'M'연산, 'J'연산, 'C'연산, 'B'연산, 'H'연산
*/


#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
using namespace std;
int R[10] = {0,}; // Register
int count = 0;
fstream * program_counter = NULL;

// Calculator Class
class Calculator{
    protected:
        char * cal_data = NULL;
        char * operation = NULL;
        char * op1 = NULL;
        char * op2 = NULL;
    public:
        Calculator(char * _data){
            cal_data = new char[strlen(_data)+1];
            strcpy(cal_data, _data);
        };

        virtual void token_data(); // 입력받은 문자열을 분리해주는 함수
        char* return_operation() const; // 분리된 Operation Return
        char* return_op1() const; // 분리된 Operand1 Return
        char* return_op2() const; // 분리된 Oerand2 Return
        virtual void select_operation(){}; // Operation에 따른 연산 선택
        virtual void move_operand(){}; // MOV Operation일 경우 Mov 연산 수행 'M'
        virtual void compare_operand(){}; // Compare Operation일 경우 Compare 연산 수행
        int jump_operand(char * line_op); // Jump Operation일 경우 Jump 연산 수행 'J'
        bool branch_operand(); // Branch Operation일 경우 Branch 연산 수행 'B'
        ~Calculator(); // Calculator Class 소멸자
};

void Calculator::token_data(){
    int i = 0;
    char ** temp_data = new char*[3];
    for(int j = 0; j<3; j++){
        temp_data[j] = new char[3];
    }
    char * token = strtok(cal_data, " ");
    
    while(token != NULL){
        strcpy(temp_data[i], token);
        token = strtok(nullptr, " ");
        i++;
    }
    operation = new char[strlen(temp_data[0])+1];
    op1 = new char[strlen(temp_data[1])+1];
    op2 = new char[strlen(temp_data[2])+1];
    strcpy(operation, temp_data[0]);
    strcpy(op1, temp_data[1]);
    strcpy(op2, temp_data[2]);

    for(int k = 0; k<3; k++){
        delete temp_data[k];
    }
}

char* Calculator::return_operation() const{
    return operation;
}
char* Calculator::return_op1() const{
    return op1;
}

char* Calculator::return_op2() const{
    return op2;
}

int Calculator::jump_operand(char * line_op){
    int line_number = line_op[0] - '0';
    return line_number;
}

bool Calculator::branch_operand(){
    if(R[0] == 0){
        return true;
    }
    else{
        return false;
    }
}

Calculator::~Calculator(){
    delete []cal_data;
}

// Operand 2개 모두 Hex Value일 경우 Calculator, Calculator를 상속받음
class Hex_Calculator:public Calculator{
    private:
        char * hex_operation = NULL;
        int hex_op1 = 0x00;
        int hex_op2 = 0x00;
        int result = 0x00;
    public:
        Hex_Calculator(char * _data, char * _operation, char * _op1, char * _op2):Calculator(_data) {
                hex_operation = new char[strlen(_operation) + 1];
                hex_operation = _operation;

                op1 = new char[strlen(_op1) + 1];
                op1 = _op1;

                op2 = new char[strlen(_op2) + 1];
                op2 = _op2;
        };
        void select_operation();
        void addition();
        void subtraction();
        void multiply();
        void division();
        ~Hex_Calculator();
};

void Hex_Calculator::select_operation(){
    std::cout<<"[------The HexaDecimal Calculation------]"<<endl;
    switch(hex_operation[0]){
        case '+':
            addition();
            break;
        case '-':
            subtraction();
            break;
        case '*':
            multiply();
            break;
        case '/':
            division();
            break;
        default:
            std::cout<<"There is no operation !!!"<<endl;
            exit(1);
    }
}

void Hex_Calculator::addition(){
    hex_op1 = stoul(op1, nullptr, 16);
    hex_op2 = stoul(op2, nullptr, 16);

    try{
        result = hex_op1 + hex_op1;
    }
    catch(exception& e){
        std::cout<<e.what()<<endl;
    }
    std::cout<<"<"<<++count<<"> "<<hex_op1<<" + "<<hex_op2<<" = "<<result<<" and 0x"<<result<<" is saved in R[0]"<<endl;
    R[0] = result;
}

void Hex_Calculator::subtraction(){
    hex_op1 = stoul(op1, nullptr, 16);
    hex_op2 = stoul(op2, nullptr, 16);

    try{    
        result = hex_op1 - hex_op2;
    }
    catch(exception& e){
        std::cout<<e.what()<<endl;
    }

    std::cout<<"<"<<++count<<"> "<<hex_op1<<" - "<<hex_op2<<" = "<<result<<" and 0x"<<result<<" is saved in R[0]"<<endl;
    R[0] = result;
}

void Hex_Calculator::multiply(){
    hex_op1 = stoul(op1, nullptr, 16);
    hex_op2 = stoul(op2, nullptr, 16);

    try{
        result = hex_op1 * hex_op2;
    }
    catch(exception& e){
        std::cout<<e.what()<<endl;
    }

    std::cout<<"<"<<++count<<"> "<<hex_op1<<" * "<<hex_op2<<" = "<<result<<" and 0x"<<result<<" is saved in R[0]"<<endl;
    
    R[0] = result;
}

void Hex_Calculator::division(){
    hex_op1 = stoul(op1, nullptr, 16);
    hex_op2 = stoul(op2, nullptr, 16);

    try{
        result = hex_op1 / hex_op2;
    }
    catch(exception& e){
        std::cout<<e.what()<<endl;
    }

    std::cout<<"<"<<++count<<"> "<<hex_op1<<" / "<<hex_op2<<" = "<<result<<" and 0x"<<result<<" is saved in R[0]"<<endl;
    R[0] = result;
}

Hex_Calculator::~Hex_Calculator(){
    delete []hex_operation;
    delete []op1;
    delete []op2;
        
}

// Operand가 모두 Register일 경우 Calculator, Calculator를 상속 받음
class Both_Reg_Calculator:public Calculator{
    private:
        char * reg_operation = NULL;
        int reg_op1 = 0x00;
        int reg_op2 = 0x00;
        int result = 0;
        int src_idx = 0;
        int dst_idx = 0;
    public:
        Both_Reg_Calculator(char * _data, char * _operation, char * _op1, char * _op2):Calculator(_data){
            reg_operation = new char[strlen(_operation)+1];
            op1 = new char[strlen(_op1)+1];
            op2 = new char[strlen(_op2)+1];

            strcpy(reg_operation, _operation);
            strcpy(op1, _op1);
            strcpy(op2, _op2);
        }

        void token_data();
        void move_operand();
        void compare_operand();
        void select_operation();
        void addition();
        void subtraction();
        void multiply();
        void division();
        ~Both_Reg_Calculator();
};

void Both_Reg_Calculator::token_data(){
    dst_idx = op1[1] - '0';
    src_idx = op2[1] - '0';
    
    // idx 범위가 정상적이지 않을 경우 예외처리
    if(dst_idx < 0 || dst_idx >9 || src_idx < 0 || src_idx >9){
        std::cout<<"Out of Range !!!"<<endl;
        exit(1);
    }
}

void Both_Reg_Calculator::move_operand(){
    std::cout<<"------The Both Move Operation------"<<endl;
    R[dst_idx] = R[src_idx];
    std::cout<<"<"<<++count<<">"<<" Moved Value is "<<R[src_idx]<<" and R["<<src_idx<<"] moves to R["<<dst_idx<<"]"<<endl;
    std::cout<<"The Dst Register R["<<dst_idx<<"]'s value is now "<<R[dst_idx]<<endl;
}

void Both_Reg_Calculator::compare_operand(){
    std::cout<<"------The Register Compare Operation------"<<endl;
    if(R[dst_idx] == R[src_idx]){
        R[0] = 0;
        std::cout<<"<"<<++count<<">"<<" Now the R[0] is "<<R[0]<<endl;
    }
    else if(R[dst_idx] > R[src_idx]){
        R[0] = 1;
        std::cout<<"<"<<++count<<">"<<" Now the R[0] is "<<R[0]<<endl;
    }
    else{
        R[0] = -1;
        std::cout<<"<"<<++count<<">"<<" Now the R[0] is "<<R[0]<<endl;
    }
}

void Both_Reg_Calculator::select_operation(){
    std::cout<<"[------The Both Reg Calculation------]"<<endl;
    switch(reg_operation[0]){
        case '+':
            addition();
            break;
        case '-':
            subtraction();
            break;
        case '*':
            multiply();
            break;
        case '/':
            division();
            break;
        default:
            std::cout<<"There is no operation !!!"<<endl;
            exit(1);
    }
}

void Both_Reg_Calculator::addition(){ 
    reg_op1 = dst_idx;
    reg_op2 = src_idx;

    try{
        R[0] = R[reg_op1] + R[reg_op2];
    }
    catch(exception& e){
        std::cout<<e.what()<<endl;
    }
    std::cout<<"<"<<++count<<">"<<" R["<<reg_op1<<"] + R["<<reg_op2<<"] = 0x"<<R[0]<<" and saved it R[0]"<<endl;
    
}

void Both_Reg_Calculator::subtraction(){ 
    reg_op1 = dst_idx;
    reg_op2 = src_idx;
    try{
        R[0] = R[reg_op1] - R[reg_op2];
    }
    catch(exception& e){
        std::cout<<e.what()<<endl;
    }
    std::cout<<"<"<<++count<<">"<<" R["<<reg_op1<<"] - R["<<reg_op2<<"] = 0x"<<R[0]<<" and saved it R[0]"<<endl;
}

void Both_Reg_Calculator::multiply(){ 
    reg_op1 = dst_idx;
    reg_op2 = src_idx;
    
    try{
        R[0] = R[reg_op1] * R[reg_op2];
    }
    catch(exception& e){
        std::cout<<e.what()<<endl;
    }

    std::cout<<"<"<<++count<<">"<<" R["<<reg_op1<<"] * R["<<reg_op2<<"] = 0x"<<R[0]<<" and saved it R[0]"<<endl;
}

void Both_Reg_Calculator::division(){ 
    reg_op1 = dst_idx;
    reg_op2 = src_idx;
    try{
        R[0] = R[reg_op1] / R[reg_op2];
    }
    catch(exception& e){
        std::cout<<e.what()<<endl;
    }
    std::cout<<"<"<<++count<<">"<<" R["<<reg_op1<<"] / R["<<reg_op2<<"] = 0x"<<R[0]<<" and saved it R[0]"<<endl;
}

Both_Reg_Calculator::~Both_Reg_Calculator(){
    delete reg_operation, op1, op2;
}

// Operand가 1개만 Register일 경우 Calculator, Calculator를 상속 받음
class Either_Reg_Calculator:public Calculator{
    private:
        char * reg_operation = NULL;
        int reg_op = 0x00;
        int op = 0x00;
        int result = 0;
        int idx = 0;
    public:
        Either_Reg_Calculator(char * _data, char * _operation, char * _op1, char * _op2):Calculator(_data){
            reg_operation = new char[strlen(_operation)+1];
            op1 = new char[strlen(_op1)+1];
            op2 = new char[strlen(_op2)+1];

            strcpy(reg_operation, _operation);
            strcpy(op1, _op1);
            strcpy(op2, _op2);
        };

        void token_data();
        void move_operand();
        void select_operation();    
        void addition();
        void subtraction();
        void multiply();
        void division();
        ~Either_Reg_Calculator();

};

void Either_Reg_Calculator::token_data(){
    if(strstr(op1, "R")){
        idx = op1[1] - '0';
    }
    else{
        idx = op2[1] - '0';
    }

    if(idx < 0 || idx > 9){
        std::cout<<"Out of Range"<<endl;
        exit(1);
    }
}

void Either_Reg_Calculator::move_operand(){
    std::cout<<"[------The Either Move Operation------]"<<endl;
    op = stoul(op2, nullptr, 16);
    R[idx] = op;
    std::cout<<"<"<<++count<<">"<<" Moved Value is 0x"<<op<<" and "<<op<<" moves to R["<<idx<<"]"<<endl;
    std::cout<<"The Dst Register R["<<idx<<"]'s value is now "<<R[idx]<<endl;
}

void Either_Reg_Calculator::select_operation(){
    std::cout<<"[------The Either Reg Calculation------]"<<endl;
    switch(reg_operation[0]){
        case '+':
            addition();
            break;
        case '-':
            subtraction();
            break;
        case '*':
            multiply();
            break;
        case '/':
            division();
            break;
        default:
            std::cout<<"There is no operation !!!"<<endl;
            exit(1);
    }
}

void Either_Reg_Calculator::addition(){
    token_data();
    reg_op = idx;
    if(strstr(op1, "R")){
        op = stoul(op2, nullptr, 16);
        try{    
            result = R[idx] + op;
        }
        catch(exception& e){
            std::cout<<e.what()<<endl;
        }
    }
    else{
        op = stoul(op1, nullptr, 16);
        try{    
            result = R[idx] + op;
        }
        catch(exception& e){
            std::cout<<e.what()<<endl;
        }
        result = R[idx] + op;
    }
    R[0] = result;
    std::cout<<"<"<<++count<<">"<< "R["<<idx<<"] + "<<op<<" = "<<result<<" and saved it R[0]"<<endl;
}

void Either_Reg_Calculator::subtraction(){
    token_data();
    reg_op = idx;
    if(strstr(op1, "R")){
        op = stoul(op2, nullptr, 16);
        try{    
            result = R[idx] - op;
        }
        catch(exception& e){
            std::cout<<e.what()<<endl;
        }
    }
    else{
        op = stoul(op1, nullptr, 16);
        try{    
            result = R[idx] - op;
        }
        catch(exception& e){
            std::cout<<e.what()<<endl;
        }
    }
    R[0] = result;
    std::cout<<"<"<<++count<<">"<< "R["<<idx<<"] - "<<op<<" = "<<result<<" and saved it R[0]"<<endl;
}

void Either_Reg_Calculator::multiply(){
    token_data();
    reg_op = idx;
    if(strstr(op1, "R")){
        op = stoul(op2, nullptr, 16);
        try{    
            result = R[idx] * op;
        }
        catch(exception& e){
            std::cout<<e.what()<<endl;
        }
    }
    else{
        op = stoul(op1, nullptr, 16);
        try{    
            result = R[idx] * op;
        }
        catch(exception& e){
            std::cout<<e.what()<<endl;
        }
    }
    R[0] = result;
    std::cout<<"<"<<++count<<">"<< "R["<<idx<<"] * "<<op<<" = "<<result<<" and saved it R[0]"<<endl;
}

void Either_Reg_Calculator::division(){
    token_data();
    reg_op = idx;
    if(strstr(op1, "R")){
        op = stoul(op2, nullptr, 16);
        try{    
            result = R[idx] / op;
        }
        catch(exception& e){
            std::cout<<e.what()<<endl;
        }
    }
    else{
        op = stoul(op1, nullptr, 16);
        try{    
            result = R[idx] / op;
        }
        catch(exception& e){
            std::cout<<e.what()<<endl;
        }
    }
    R[0] = result;
    std::cout<<"<"<<++count<<">"<< "R["<<idx<<"] / "<<op<<" = "<<result<<" and saved it R[0]"<<endl;
}

Either_Reg_Calculator::~Either_Reg_Calculator(){
    delete reg_operation, op1, op2;
}

int main(){
    fstream input_file("input.txt", ios::in);
    fstream* temp_pointer;
    int jump_count = 0, branch_count = 0;
    int total_jump_count = 0, total_branch_count = 0;
    if(input_file.fail() == true ){
        std::cout<<"Fail to the open file"<<endl;
        exit(1);
    }

    char * my_data = new char[100];

    // Operation이 'J' 또는 'B'일때 무한 Loop를 방지하기 위한 반복문 (count를 증가시킴으로써 방지해줌)
    while(input_file.eof() == false){
        input_file.getline(my_data, 100, '\n');
        if(my_data[0] == 'J'){
            total_jump_count++;
        }  
        else if(my_data[0] == 'B'){
            total_branch_count++;
        }
    }
    input_file.seekg(0);
    while(input_file.eof() == false){
        input_file.getline(my_data, 100, '\n');
        program_counter = &input_file;
        
        Calculator * calculator = new Calculator(my_data);
        
        calculator->token_data();
        char * operation = calculator->return_operation();
        char * op1 = calculator->return_op1();
        char * op2 = calculator->return_op2();

        Calculator * hex_calculator = new Hex_Calculator(my_data, operation, op1, op2); 
        Calculator * both_reg_calculator = new Both_Reg_Calculator(my_data, operation, op1, op2);
        Calculator * either_reg_calculator = new Either_Reg_Calculator(my_data, operation, op1, op2);

        // Operation이 사칙연산일 경우 아래 연산 수행
        if(my_data[0] == '+'||my_data[0] == '-'||my_data[0] == '*'||my_data[0] == '/'){
            if(strstr(op1, "0x") && strstr(op2, "0x")){ 
                hex_calculator->token_data();
                hex_calculator->select_operation();
            }
            else if(strstr(op1, "R") && strstr(op2, "R")){
                both_reg_calculator->token_data();
                both_reg_calculator->select_operation();
            }
            else{
                either_reg_calculator->token_data();
                either_reg_calculator->select_operation();
            }
        }

        // Operation이 'M'일 겨우 아래 연산 수행
        else if(my_data[0] == 'M'){ 
            if(strstr(op1, "R") && strstr(op2, "R")){
                both_reg_calculator->token_data();
                both_reg_calculator->move_operand();
            }
            else if(strstr(op1, "R") && strstr(op2, "0x")){
                either_reg_calculator->token_data();
                either_reg_calculator->move_operand();
            }
        }
        // Operation이 'C'일 겨우 아래 연산 수행
        else if(my_data[0] == 'C'){
            both_reg_calculator->token_data();
            both_reg_calculator->compare_operand();
        }
        // Operation이 'J'일 겨우 아래 연산 수행
        else if(my_data[0] == 'J' && jump_count != total_jump_count){
            cout<<"-------Jump Instruction !!!-------"<<endl;
            int file_idx = calculator->jump_operand(op1);
            input_file.seekg(0);
            for(int i = 0; i<file_idx-1; i++){
               input_file.getline(my_data, 100, '\n');
            }
            jump_count++;
        }
        // Operation이 'B'일 겨우 아래 연산 수행
        else if(my_data[0] == 'B' && branch_count != total_branch_count){
            cout<<"-------Branch Instruction !!!-------"<<endl;
            bool branch = calculator->branch_operand();
            if(branch){
                int branch_idx = op1[0] - '0';
                input_file.seekg(0);
                for(int i = 0; i<branch_idx-1; i++){
                    input_file.getline(my_data, 100, '\n');
                }
                branch_count++;
            }
        }
        else if(my_data[0] == 'H'){    
            // 프로그램 종료를 알림
            std::cout<<"====================================="<<endl;
            std::cout<<"Finish the Simple Calculation Program."<<endl;
            std::cout<<"====================================="<<endl;
            break;
        }
        // 올바르지 않은 연산일 경우 아래 연산 수행
        else{
            if(total_jump_count != jump_count && total_branch_count != branch_count){    
                std::cout<<"There is no Operation !!!"<<endl;
                exit(1);
            }
        }
        delete calculator, hex_calculator, both_reg_calculator, either_reg_calculator;
    }

    delete []my_data;
    input_file.close();
    return 0;
}