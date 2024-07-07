#include <iostream>
#include <string>
#include <vector>
#include <stdlib.h>
#include <fstream>
using namespace std;
int stringToInt(string &str)
{
    int result = 0;
    int sign = 1;
    int j = 0;

    if (str[0] == '-')
    {
        sign = -1;
        j = 1;
    }
    for (int i = j; i < str.size(); i++)
    {
        if (str[i] >= '0' && str[i] <= '9')
        {
            result = result * 10 + (str[i] - '0');
        }
    }
    return result * sign;
}

int opcode(unsigned int d)
{
    return ((d << 25) >> 25);
}
int rd(unsigned int d)
{
    return ((d << 20) >> 27);
}
int funct3(unsigned int d)
{
    return (d << 17) >> 29;
}
int rs2(unsigned int d)
{
    return (d << 7) >> 27;
}
int rs1(unsigned int d)
{
    return (d << 12) >> 27;
}
int funct7(unsigned int d)
{
    return (d >> 25);
}

int imm_i_110(unsigned int d)
{
    return ((int)d) >> 20;
}

int imm_i_116(unsigned int d)
{
    return ((int)d) >> 26;
}
int imm_i_50(unsigned int d)
{
    return (d << 6) >> 26;
}
int imm_s_110(unsigned int d)
{
    int d1 = (d >> 25) << 5;
    int d2 = ((d >> 7) << 27) >> 27;
    return (int)(d1 + d2);
}
int imm_b_110(unsigned int d)
{
    int c = (int)d;
    int d1 = (((d >> 7) << 31) >> 21);
    int d2 = ((d >> 8) << 28) >> 28;
    int d3 = (((d >> 25) << 26) >> 22);
    int d4 = ((c >> 31) << 11);
    return ((int)(d1 + d2 + d3 + d4) << 1);
}
int imm_u(unsigned int d)
{
    int d1 = (d >> 12);
    return (int)d1;
}
int imm_j(unsigned int d)
{
    int c = (int)d;
    int d1 = ((d >> 12) << 24) >> 13;
    int d2 = ((d >> 20) << 31) >> 21;
    int d3 = ((d >> 21) << 22) >> 22;
    int d4 = ((c >> 31) << 19);

    return (int)(d1 + d2 + d3 + d4) << 1;
}

int main()
{
    string filename;
    cout << "Enter the name of the .txt file: ";
    cin >> filename;

    ifstream file(filename);
    if (!file.is_open())
    {
        cout << "Error opening the file. Exiting...";
        return 1;
    }

    vector<string> output;
    string line;
    int no = 1;

    while (file >> line)
    {
        string r1, r2, de;
        unsigned int d = stoul(line, 0, 16);
        int op = opcode(d);
        r1 = to_string(rs1(d));
        r2 = to_string(rs2(d));
        de = to_string(rd(d));
        string inst = "";
        // cout<<op<<endl;
        if (op == 99)
        {
            int offset = imm_b_110(d) / 4;
            string off = to_string(offset);
            if (funct3(d) == 0)
            {
                inst += "beq ";
            }
            else if (funct3(d) == 1)
            {
                inst += "bneq ";
            }
            else if (funct3(d) == 4)
            {
                inst += "blt ";
            }
            else if (funct3(d) == 5)
            {

                inst += "bge ";
            }
            else if (funct3(d) == 6)
            {

                inst += "bltu ";
            }
            else if (funct3(d) == 7)
            {

                inst += "bgeu";
            }
            inst += "x" + r1 + ", " + "x" + r2 + ", L" + off;
            output.push_back(inst);
        }

        if (op == 111)
        {

            int offset = imm_j(d) / 4;
            string off = to_string(offset);
            inst += "jal x" + de + ", L" + off;
            output.push_back(inst);
        }

        if (op == 103)
        {

            int imm = imm_i_110(d);
            string i = to_string(imm);

            inst += "jalr x" + de + ", " + i + "(x" + r1 + ")";

            output.push_back(inst);
        }
    

    if (op == 55)
    {
        char hex[50];
        sprintf(hex, "%x", imm_u(d));
        inst += "lui x" + de + ",0x" + hex;
        output.push_back(inst);
    }
    if (op == 23)
    {
        char hex[50];
        sprintf(hex, "%x", imm_u(d));
        inst += "auipc x" + de + ",0x" + hex;
        output.push_back(inst);
    }

    if (op == 51)
    {

        if (funct7(d) == 0 && funct3(d) == 0)
        {
            inst += "add ";
        }
        if (funct7(d) == 32 && funct3(d) == 0)
        {
            inst += "sub ";
        }
        if (funct7(d) == 0 && funct3(d) == 4)
        {
            inst += "xor ";
        }
        if (funct7(d) == 0 && funct3(d) == 6)
        {
            inst += "or ";
        }
        if (funct7(d) == 0 && funct3(d) == 7)
        {
            inst += "and ";
        }
        if (funct7(d) == 0 && funct3(d) == 1)
        {
            inst += "sll ";
        }
        if (funct7(d) == 0 && funct3(d) == 5)
        {
            inst += "srl ";
        }
        if (funct7(d) == 32 && funct3(d) == 5)
        {
            inst += "sra ";
        }
        if (funct7(d) == 0 && funct3(d) == 2)
        {
            inst += "slt ";
        }
        if (funct7(d) == 0 && funct3(d) == 3)
        {
            inst += "sltu ";
        }
        inst += "x" + de + ", " + "x" + r1 + ",x" + r2;

        output.push_back(inst);
    }
    if (op == 19)
    {
        int imm1 = imm_i_110(d);
        string i1 = to_string(imm1);
        int imm2 = imm_i_50(d);
        string i2 = to_string(imm2);

        if (funct3(d) == 0)
        {
            inst += "addi x" + de + ", x" + r1 + ", " + i1;
        }
        if (funct3(d) == 4)
        {
            inst += "xori x" + de + ", x" + r1 + ", " + i1;
        }
        if (funct3(d) == 6)
        {
            inst += "ori x" + de + ", x" + r1 + ", " + i1;
        }
        if (funct3(d) == 7)
        {
            inst += "andi x" + de + ", x" + r1 + ", " + i1;
        }
        if (funct3(d) == 1 && imm_i_116(d) == 0)
        {
            inst += "slli x" + de + ", x" + r1 + ", " + i2;
        }
        if (funct3(d) == 5 && imm_i_116(d) == 0)
        {
            inst += "srli x" + de + ", x" + r1 + ", " + i2;
        }
        if (funct3(d) == 5 && imm_i_116(d) == 16)
        {
            inst += "srai x" + de + ", x" + r1 + ", " + i2;
        }
        if (funct3(d) == 2)
        {
            inst += "slti x" + de + ", x" + r1 + ", " + i1;
        }
        if (funct3(d) == 3)
        {
            inst += "sltiu x" + de + ", x" + r1 + ", " + i1;
        }
        output.push_back(inst);
    }
    if (op == 3)
    {
        int imm = imm_i_110(d);
        string i = to_string(imm);

        if (funct3(d) == 0)
        {
            inst += "lb x" + de + ", " + i + "(x" + r1 + ")";
        }
        if (funct3(d) == 1)
        {
            inst += "lh x" + de + ", " + i + "(x" + r1 + ")";
        }
        if (funct3(d) == 2)
        {
            inst += "lw x" + de + ", " + i + "(x" + r1 + ")";
        }
        if (funct3(d) == 3)
        {
            inst += "ld x" + de + ", " + i + "(x" + r1 + ")";
        }
        if (funct3(d) == 4)
        {
            inst += "lbu x" + de + ", " + i + "(x" + r1 + ")";
        }
        if (funct3(d) == 5)
        {
            inst += "lhu x" + de + ", " + i + "(x" + r1 + ")";
        }
        if (funct3(d) == 6)
        {
            inst += "lwu x" + de + ", " + i + "(x" + r1 + ")";
        }
        output.push_back(inst);
    }

    if (op == 35)
    {

        int imm = imm_s_110(d);
        string i = to_string(imm);
        if (funct3(d) == 0)
        {
            inst += "sb x" + r2 + ", " + i + "(x" + r1 + ")";
        }
        if (funct3(d) == 1)
        {
            inst += "sh x" + r2 + ", " + i + "(x" + r1 + ")";
        }
        if (funct3(d) == 2)
        {
            inst += "sw x" + r2 + ", " + i + "(x" + r1 + ")";
        }
        if (funct3(d) == 3)
        {
            inst += "sd x" + r2 + ", " + i + "(x" + r1 + ")";
        }
        output.push_back(inst);
    }

    no++;
}



int label = 1;
for (int i = 0; i < output.size(); i++)
{
    string instuct = output[i];
    if (instuct[0] == 'b' || instuct[0] == 'j')
    {
        int value = instuct.find('L');
        string str = instuct.substr(value + 1, instuct.length());
        int result = stringToInt(str);
        int jump = i;
        jump += result;
        if (jump >= 0 && jump < output.size())
        {
            string Jumpinstruct = output[jump];

            if (Jumpinstruct[0] == 'L')
            {
                int val = Jumpinstruct.find(':');
                output[i] = instuct.substr(0, value) + Jumpinstruct.substr(0, val);
            }
            else
            {
                string str = "L" + to_string(label);
                output[jump] = str + ": " + Jumpinstruct;
                output[i] = instuct.substr(0, value) + str;
                label++;
            }
        }
    }
}
for (auto instruction : output)
{
    cout << instruction << endl;
}

return 0;
}
