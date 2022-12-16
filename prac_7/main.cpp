#include <iostream>
#include <cmath>
#include <fstream>
#include <algorithm>
#include <vector>
#include <map>
#include <set>
#include <iomanip>

const int BUFFER_SIZE = 5;

const std::string zero_str = "$";
int write_counter_buff = 0, read_counter_buff = 0;
uint8_t write_buff = 0, read_buff = 0;

std::ifstream fin;
std::ofstream fout;

void print_to_file(uint8_t value)
{
    write_buff += value << (7 - write_counter_buff);
    write_counter_buff++;
    if (write_counter_buff == 8)
    {
        fout.open("zipped.bin", std::ios::app | std::ios::binary);
        fout.write((char*)&write_buff, sizeof(write_buff));
        fout.close();
        write_buff = 0;
        write_counter_buff = 0;
    }
}
uint8_t read_from_file()
{
    if (read_counter_buff == 0)
    {
        read_counter_buff = 8;
        fin.read((char*)&read_buff, sizeof(read_buff));
    }
    read_counter_buff--;
    uint8_t value = read_buff;
    read_buff >> 1;
    value = (value >> read_counter_buff) & 1;
    return value;
}
//-----------------------------------------------------------------RLE--------------------------------------------------------

std::vector<std::pair<int, char>> rle_encoder(const std::string& str)
{
    std::vector<std::pair<int, char>> ans;

    bool ff = false;
    int cnt = 0;
    for (int i = 0; i < str.length() - 1; ++i)
    {
        cnt++;
        if (str[i] == str[i + 1])
            ff = true;
        else
            ff = false;
        if (!ff)
        {
            std::cout << cnt << "." << str[i] << " ";
            ans.push_back(std::make_pair(cnt, str[i]));
            cnt = 0;
        }
    }
    cnt++;
    std::cout << cnt << "." << str.back() << std::endl;
    ans.push_back(std::make_pair(cnt, str.back()));
    return ans;
}

std::string rle_decoder(const std::vector<std::pair<int, char>>& v)
{
    std::string ans = "";
    for (int i = 0; i < v.size(); ++i)
    {
        ans.append(v[i].first, v[i].second);
    }
    return ans;
}

//---------------------------------------------------------LZ77---------------------------------------------------------------

struct LZ77_NODE
{
    int offset, length;
    char next;
};

int find_length(int buff_begin, int buff_end, const std::string& str)
{
    int str_pos = buff_end, buff_pos = buff_begin;
    while (str_pos < str.length() && str[buff_pos] == str[str_pos])
    {
        buff_pos++;
        str_pos++;
        if (buff_pos == buff_end)
            buff_pos = buff_begin;
    }
    return str_pos - buff_end;
}

std::pair<int, int> find_match(int buff_begin, int buff_end, const std::string& str)
{
    int pos = buff_end, max = 0, tmp = 0;
    for (int i = buff_begin; i < buff_end; ++i)
    {
        tmp = find_length(i, buff_end, str);
        if (tmp > max || tmp == max && max != 0)
        {
            max = tmp;
            pos = i;
        }
    }
    return {buff_end - pos, max};
}

std::vector<LZ77_NODE> lz77_encoder(const std::string& str)
{
    std::vector<LZ77_NODE> ans;

    int buff_begin = 0, buff_end = 0;

    while (buff_end < str.length())
    {
        auto [offset, length] = find_match(buff_begin, buff_end, str);
        ans.push_back({offset, length, ((buff_end + length < str.length()) ? str[buff_end + length] : '\0')});
        buff_end += length + 1;
        buff_begin = std::max(buff_end - BUFFER_SIZE, 0);
    }
    return ans;
}

void print_lz77(const std::vector<LZ77_NODE>& vector)
{
    std::cout << "[";
    for (int i = 0; i < vector.size(); ++i)
    {
        if (i > 0)
            std::cout << ",";
        std::cout << "(" << vector[i].offset << "," << vector[i].length << "," << vector[i].next << ")";
    }
    std::cout << "]" << std::endl;
}

//---------------------------------------------------------LZ78---------------------------------------------------------------

int dictionary_find_match(const std::vector<std::string> dic, const std::string& str)
{
    for (int i = 0; i < dic.size(); ++i)
    {
        if (dic[i] == str)
            return i + 1;
    }
    return 0;
}

std::vector<std::pair<int, char>> lz78_encoder(const std::string& str)
{
    int prev_pos = 0, curr_pos = 0;

    std::vector<std::pair<int, char>> ans;
    std::vector<std::string> dic;
    int cnt = 0;
    int str_pos = 0;

    while (str_pos + cnt < str.length())
    {
        prev_pos = curr_pos;
        curr_pos = dictionary_find_match(dic, str.substr(str_pos, cnt + 1));
        if (curr_pos == 0)
        {
            dic.push_back(str.substr(str_pos, cnt + 1));
            ans.push_back({prev_pos, str[str_pos + cnt]});
            prev_pos = 0;
            str_pos += cnt + 1;
            cnt = 0;
        }
        else
            cnt++;
    }

    if (cnt != 0)
    {
        ans.push_back({curr_pos, '\0'});
    }
    return ans;
}

void print_lz78(const std::vector<std::pair<int, char>>& vector)
{
    std::cout << "[";
    for (int i = 0; i < vector.size(); ++i)
    {
        if (i > 0)
            std::cout << ",";
        std::cout << "(" << vector[i].first << "," << vector[i].second << ")";
    }
    std::cout << "]" << std::endl;
}

//----------------------------------------------------HUFFMAN---------------------------------------------------------------

struct Huff_node
{
    std::string value = "";
    int cnt = 0;
    Huff_node *left = nullptr, *right = nullptr;
    Huff_node(std::string value, int cnt)
    {
        this->value = value;
        this->cnt = cnt;
    }
};

struct cmp
{
    bool operator()(const Huff_node* const a, const Huff_node* const b) const
    {
        if (a->cnt == b->cnt)
        {
            return a->value < b->value;
        }
        return a->cnt < b->cnt;
    }
};

std::map<char, int> cnt_table(const std::string& str)
{
    std::map<char, int> mapa;
    for (int i = 0; i < str.length(); ++i)
    {
        mapa[str[i]]++;
    }
    return mapa;
}

Huff_node* tree_builder(const std::string& str)
{
    Huff_node* root = nullptr;

    std::set<Huff_node*, cmp> set;

    std::map<char, int> mapa;

    for (int i = 0; i < str.length(); ++i)
    {
        mapa[str[i]]++;
    }

    mapa['$']++;

    for (auto [value, cnt] : mapa)
    {
        std::string tmp1 = ".";
        tmp1[0] = value;
        Huff_node* node = new Huff_node(tmp1, cnt);
        set.insert(node);
    }

    while (set.size() > 1)
    {
        Huff_node *a, *b;
        a = *set.begin();
        set.erase(set.begin());
        b = *set.begin();
        set.erase(set.begin());

        Huff_node* tmp = new Huff_node(a->value + b->value, a->cnt + b->cnt);
        tmp->left = a;
        tmp->right = b;
        set.insert(tmp);
    }

    return *set.begin();
}

void find_leaf(const Huff_node* node, std::map<std::string, std::string>& mapa, std::string& str)
{
    if (!node->left && !node->right)
    {
        mapa[node->value] = str;
        return;
    }
    str.append("0");
    find_leaf(node->left, mapa, str);
    str.pop_back();
    str.append("1");
    find_leaf(node->right, mapa, str);
    str.pop_back();
    return;
}

std::map<std::string, std::string> get_code_table(const std::string& str)
{
    std::map<std::string, std::string> mapa;
    Huff_node* root = tree_builder(str);
    std::string tmp = "";

    find_leaf(root, mapa, tmp);

    return mapa;
}

void print_tree(Huff_node* node, int height, std::string& str, int depth = 0)
{
    if (!node)
    {
        if (depth < height)
        {
            std::cout << "|";
            for (int i = 0; i < height; ++i)
            {
                std::cout << "           |";
            }
            std::cout << std::endl;
        }
        return;
    }
    str.append("1");
    print_tree(node->right, height, str, depth + 1);
    str.pop_back();
    std::cout << "|";
    for (int i = 0; i < depth; ++i)
    {
        std::cout << "           |";
    }
    std::string tmp1 = node->value;
    std::sort(tmp1.begin(), tmp1.end());
    std::cout << std::left << std::setw(5) << tmp1 << (tmp1.size() == 0 ? "" : " ");
    if (str.size() != 0)
        std::cout << std::setw(3) << str << " " << node->cnt << "|";
    else
        std::cout << std::right << std::setw(3) << str << std::left << node->cnt << "|";

    if (depth < height - 1)
        std::cout << "-----------|";
    for (int i = depth + 2; i < height; ++i)
    {
        std::cout << "===========|";
    }
    std::cout << std::endl;
    str.append("0");
    print_tree(node->left, height, str, depth + 1);
    str.pop_back();
}
int find_height(Huff_node* node)
{
    if (!node)
    {
        return 0;
    }
    int right = find_height(node->right);
    int left = find_height(node->left);
    return std::max(right, left) + 1;
}
void print_tree_nice(const std::string& str)
{

    Huff_node* root = tree_builder(str);

    int height = find_height(root);

    for (int i = 0; i < height; ++i)
    {
        std::cout << "============";
    }
    std::cout << "=" << std::endl;

    std::string tmp = "";

    print_tree(root, height, tmp);

    for (int i = 0; i < height; ++i)
    {
        std::cout << "============";
    }
    std::cout << "=" << std::endl;
}

void huffman_encode(const std::string& str)
{
    fout.open("zipped.bin", std::ios::out | std::ios::binary);
    fout.close();
    std::map<std::string, std::string> mapa = get_code_table(str);
    std::string tmp = "";
    for (int i = 0; i < str.size(); ++i)
    {
        tmp = str[i];
        tmp = mapa[tmp];
        while (tmp.size() > 0)
        {
            if (tmp.front() == '1')
                print_to_file(1);
            else
                print_to_file(0);
            tmp.erase(tmp.begin());
        }
    }
    tmp = mapa[zero_str];
    while (tmp.size() > 0)
    {
        if (tmp.front() == '1')
            print_to_file(1);
        else
            print_to_file(0);
        tmp.erase(tmp.begin());
    }
    while (write_counter_buff > 0)
    {
        print_to_file(0);
    }
}

std::string huffman_decode(const std::string& str)
{
    std::string ans = "";
    Huff_node* root = tree_builder(str);
    Huff_node* curr = root;

    fin.open("zipped.bin", std::ios::in | std::ios::binary);
    while (!fin.eof() || read_counter_buff > 0)
    {
        int tmp1 = read_from_file();
        if (tmp1 == 0)
        {
            curr = curr->left;
        }
        else
        {
            curr = curr->right;
        }
        if (!curr->left)
        {
            if (curr->value == zero_str)
            {
                read_buff = 0;
                read_counter_buff = 0;
                break;
            }
            ans += curr->value;
            curr = root;
        }
    }

    fin.close();

    return ans;
}

void encode_file()
{
    std::ifstream ffin("unzipped.txt", std::ios::in);

    std::string buf = "", tmp;
    while (!ffin.eof())
    {
        std::getline(ffin, tmp);
        buf += tmp;
    }

    std::map<std::string, std::string> mapa = get_code_table(buf);
    for (auto& [value, code] : mapa)
    {
        std::cout << value << " " << code << std::endl;
    }

    huffman_encode(buf);
    std::cout << huffman_decode(buf) << std::endl;
}

//----------------------------------------------------Shannon-Fano---------------------------------------------------------------
void add_node(Huff_node*& node, const std::vector<std::pair<std::string, int>>& vec, const int& begin, const int& end)
{
    std::string tmp2 = "";
    int sum = 0;
    for (int i = begin; i < end; ++i)
    {
        tmp2 += vec[i].first;
        sum += vec[i].second;
    }
    node->value = tmp2;
    node->cnt = sum;
    if (end - begin == 1)
        return;
    int dif = sum / 2;
    for (int i = begin; i < end; ++i)
    {
        dif -= vec[i].second;
        if (dif < 0)
        {
            Huff_node* tmp = new Huff_node("none", 0);
            node->left = tmp;
            add_node(tmp, vec, begin, i);
            Huff_node* tmp1 = new Huff_node("none", 0);
            node->right = tmp1;
            add_node(tmp1, vec, i, end);
            return;
        }
    }
}

Huff_node* build_tree(const std::string& str)
{
    Huff_node* root = new Huff_node("none", 0);
    std::map<char, int> mapa;
    std::vector<std::pair<std::string, int>> vector;
    for (int i = 0; i < str.size(); ++i)
    {
        mapa[str[i]]++;
    }
    for (auto [value, cnt] : mapa)
    {
        std::string tmp1;
        tmp1 = value;
        vector.push_back(std::make_pair(tmp1, cnt));
    }
    std::sort(vector.begin(),
              vector.end(),
              [](auto& left, auto& right)
              {
                  if (left.second != right.second)
                      return left.second < right.second;
                  return left.first < right.first;
              });
    add_node(root, vector, 0, vector.size());
    return root;
}

double average_code_length(const std::string& str)
{
    double ans = 0;
    int cnt = 0;

    auto mapa = get_code_table(str);
    for (auto& [value, code] : mapa)
    {
        ans += code.length();
        cnt++;
    }
    ans = ans / cnt;

    return ans;
}

double dis(const std::string& str)
{
    double average = 0, ans = 0;
    int cnt = 0;

    auto mapa = get_code_table(str);
    for (auto& [value, code] : mapa)
    {
        average += code.length();
        cnt++;
    }
    average = average / cnt;
    double dif = 0;

    for (auto& [value, code] : mapa)
    {
        if (std::abs(code.length() - average) > dif)
            dif = std::abs(code.length() - average);
    }
    ans = dif * dif;
    return ans;
}

int main()
{
    setlocale(LC_ALL, "RU");
    // rle-part
    std::string str;
    std::getline(std::cin, str);
    // std::cin >> str;
    // std::vector<std::pair<int, char>> vector = rle_encoder(str);
    // std::cout << rle_decoder(vector) << std::endl;

    // lz77-part
    // print_lz77(lz77_encoder(str));

    // lz78-part
    // print_lz78(lz78_encoder(str));

    // shannon–fano-part
    Huff_node* root = build_tree(str);
    // std::cout << "total length: " << root->cnt << std::endl;
    // std::cout << "#####################################" << std::endl;
    // print_tree_nice(str);
    // std::cout << "##########################" << std::endl;

    // huffman-part
    // str = "Antonov Danila Sergeevich";
    // Huff_node* root = tree_builder(str);
    std::map<std::string, std::string> mapa = get_code_table(str);
    for (auto& [value, code] : mapa)
    {
        std::cout << value << " " << code << std::endl;
    }

    std::cout << "#####################################" << std::endl;

    std::map<char, int> mapa1 = cnt_table(str);
    for (auto& [value, cnt] : mapa1)
    {
        std::cout << value << " " << cnt << std::endl;
    }

    std::cout << "average code length: " << average_code_length(str) << std::endl;
    std::cout << "dispersion: " << dis(str) << std::endl;
    std::cout << "#####################################" << std::endl;
    print_tree_nice(str);
    std::cout << "##########################" << std::endl;
    // huffman_encode(str);
    // std::cout << huffman_decode(str) << std::endl;
    // encode_file();
    system("pause");

    return 0;
}