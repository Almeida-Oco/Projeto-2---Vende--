#include "..\Headers\Trans.h"
#include "..\Headers\Supermarket.h"
#include "..\Headers\Product.h"
#include "..\Headers\Visualize.h"
#include "..\Headers\Input_Asker.h"
#include "..\Headers\Client.h"
#include "..\Headers\Bottom_10.h"

using namespace std;

void Supermarket::Trans::startUp()
{
	unsigned int vector_size = 0;
	bool failed = false;
	vector<string> tokens;
	Trans_t transaction;
	string line;
	ifstream fin;
	do {
		failed = false;
		cout << "Insert the transactions file name" << endl;
		getline(cin, this->trans_file_name);
		fin.open(this->trans_file_name);
		while (!fin.is_open())
		{
			cout << "File not found" << endl;
			exit(0);
			break;
		}

		if (fin >> vector_size)
			info_trans.reserve(vector_size);
		fin.ignore(999, '\n');
		fin.clear();

		while (getline(fin, line))
		{//fazer um get do numero , data e produtos;
			tokens = string_split(line, " ; ");
			if (tokens.size() != 3)
			{
				failed = true;
				break;
			}
			if (isalpha(tokens.at(2).at(0)))
			{
				transaction.products = string_split(tokens.at(2), ",");
				for (unsigned int i = 0; i<transaction.products.size(); i++)
					transaction.products.at(i) = trim(transaction.products.at(i));
			}
			else
			{
				failed = true;
				break;
			}
			vector<string> date_tokens = string_split(tokens.at(1), "/");
			transaction.date.day = stoi(date_tokens.at(0));
			transaction.date.month = stoi(date_tokens.at(1));
			transaction.date.year = stoi(date_tokens.at(2));
			transaction.number = stoi(tokens.at(0));
			this->info_trans.push_back(transaction);
		}
		if (failed)
		{
			failed = false;
			cout << "Not the transactions file, please try again" << endl;
			this->info_trans.clear();
			continue;
		}

		fin.close();
	} while (failed); //it if is the transaction files it copies its contents to a vector of structs

	sort(this->info_trans.begin(), this->info_trans.end());
}

//====================================================================================
//=================================== MODIFIERS ======================================
//====================================================================================

void Supermarket::Trans::addTrans()//adds a new transaction to the vector of transactions
{
	Trans_t T;
	T.number = Input_Asker::instance()->ask_c_number();
	T.date = Input_Asker::instance()->askDate(2);
	vector <string> prod_bought;
	string prod;
	prod_bought = askProduct( (prod_bought.size() == 0) ? true : false);
	Client::instance()->addMoney( Client::instance()->NumtoName(T.number) , Product::instance()->getPrice(prod_bought) );
	T.products = prod_bought;
	binaryInsert(T, info_trans);
}

//====================================================================================
//================================== VISUALIZERS =====================================
//====================================================================================

void Supermarket::Trans::visAllTrans() const
{
	transHeader();
	for (Trans_t T : this->info_trans)
		visTrans(T);

	cout << endl << "========================================================" << endl;
}

void Supermarket::Trans::visClientTrans() const
{
	unsigned int c_number;
	string name;
	vector<Client_t>::iterator it;
	bool first = true;
	do
	{
		name = Input_Asker::instance()->askClientName();
		it = Client::instance()->nameBinarySearch(name , Client::instance()->getInfo());
		if (it->name == name)
			c_number = it->number;
		else
			continue;
		for (Trans_t T : this->info_trans)
		{
			if (T.number == c_number)
			{
				if (first) 
					transHeader();
				first = false;
				visTrans(T);
			}
		}
		cout << endl << "========================================================" << endl;
	} while (c_number == -1);
}

void Supermarket::Trans::visDayTrans() const
{
	Date_t D;
	bool found = false, first = true;
	unsigned int i = 0;
	D = Input_Asker::instance()->askDate(2);

	for (Trans_t T : this->info_trans)
	{
		if (T.date.day == D.day && T.date.month == D.month && T.date.year == D.year)
		{
			if (first)
				transHeader();
			first = false;
			Visualize::instance()->visNumber(T.number); visDate(T.date); cout << Visualize::instance()->P_comma(T.products); cout << endl;
			found = true;
		}
		i++;
	}
	if (!found)
		cout << "The date is not on records" << endl;
	cout << endl << "========================================================" << endl;
}

void Supermarket::Trans::visBetweenDates() const
{
	bool first = true;
	Date_t lower_date = Input_Asker::instance()->askDate(0);
	Date_t upper_date = Input_Asker::instance()->askDate(1);

	for (Trans_t T : this->info_trans)
	{
		if (T.date > lower_date && T.date < upper_date)
		{
			if (first)
				transHeader();
			first = false;
			visTrans(T);
		}
	}
	cout << endl << "========================================================" << endl;
}

void Supermarket::Trans::transHeader() const
{
	cout << endl << "========================================================" << endl;
	cout << setw(NUM_BOX) << left << "Num :" << setw(DATE_BOX) << "Date :" << setw(PROD_BOX) << "Products :" << endl << endl;
}

void Supermarket::Trans::visDate(Date_t date) const {
	cout << setw(DATE_BOX) << left << to_string(date.day) + "/" + to_string(date.month) + "/" + to_string(date.year);
}

void Supermarket::Trans::visTrans(const Trans_t &T_t) const
{
	Visualize::instance()->visNumber(T_t.number); visDate(T_t.date); cout << Visualize::instance()->P_comma(T_t.products); cout << endl;
}

//====================================================================================
//================================== ADVERTISERS =====================================
//====================================================================================
vector<string> Supermarket::Trans::mostBought(vector<string> &p_bought)
//receives unsorted repetitions of different products and returns vector with products with more repetitions
{
	sort(p_bought.begin(), p_bought.end());
	int rep = 1, max_number = 0, vec_size = p_bought.size();
	vector<string> amount;
	for (int i = 0; i<vec_size; i++)
	{
		if (i == p_bought.size() - 2)
		{
			if (p_bought.at(i) == p_bought.at(i + 1))
				rep++;
			else
			{
				if (rep > max_number)
				{
					max_number = rep;
					amount.clear();
					amount.push_back(p_bought.at(i));
					rep = 1;
				}
				if (rep == max_number)
				{
					amount.push_back(p_bought.at(i));
					rep = 1;
				}
				i++;
			}
		}

		if (rep > max_number)
		{
			max_number = rep;
			amount.clear();
			amount.push_back(p_bought.at(i));
		}
		else if (rep == max_number)
		{
			amount.push_back(p_bought.at(i));
		}
		if (i == vec_size - 1)
			break;
		else if (p_bought.at(i) != p_bought.at(i + 1))
			rep = 1;
		else
			rep++;

	}
	return amount;
}//return vector with the most bought products by the most similar clients

int Supermarket::Trans::searchID_transactions(unsigned int p)
{
	unsigned int i = 0;

	while (i<Bottom_10::instance()->getCtoT().size())
	{
		if (Bottom_10::instance()->getCtoT()[i].first == p)         //with this function, I can know the line I should access in the bidimensional vector that I create in Func11.
		{
			return i;
		}
		else
			i++;
	}

	return -1;
}

void Supermarket::Trans::selectiveAd() 
{
	unsigned int target_position = searchID_transactions(Input_Asker::instance()->ask_c_number());
	
	bool verifi = false;
	vector< vector<bool> > publi(Bottom_10::instance()->getCtoT().size(), vector<bool>(Product::instance()->getSize()));
	vector<bool> vec_bool;

	for (unsigned int trans_prods = 0; trans_prods < Bottom_10::instance()->getCtoT().size(); trans_prods++ , vec_bool.clear())
	{
		for (unsigned int prods_i=0; prods_i < Product::instance()->getSize(); prods_i++)
		{
			for (unsigned int i=0; i < Bottom_10::instance()->getCtoT().at(trans_prods).second.size(); i++)
			{
				if (Product::instance()->getProd(prods_i) == (Bottom_10::instance()->getCtoT().at(trans_prods)).second.at(i))
				{
					vec_bool.push_back(true);
					verifi = true;
					break;
				}
			}

			if (!verifi)
				vec_bool.push_back(false);

			verifi = false;
		}

		publi.at(trans_prods) = vec_bool;
	}

	int max = -1, compare = 0;
	vector <string> vec_different_prod, vec_different_prod_aux;
	string product_wanted;

	for (unsigned int line = 0; line < publi.size(); line++ , compare = 0)
	{
		if (line == target_position)
			continue;
		for (unsigned int column = 0; column < Product::instance()->getSize(); column++)
		{
			if (publi[line][column] && publi[target_position][column])
				compare++;

			else if (publi[line][column] && !publi[target_position][column])
				vec_different_prod_aux.push_back(Product::instance()->getProd(column));

		}


		if (compare < max && vec_different_prod_aux.size() != 0)
			vec_different_prod_aux.clear();
		else if (vec_different_prod_aux.size() != 0 && compare > max)
		{
			vec_different_prod.clear();
			vec_different_prod = vec_different_prod_aux;
			vec_different_prod_aux.clear();
			max = compare;
		}
		else if (vec_different_prod_aux.size() != 0)
		{
			vec_different_prod.insert(vec_different_prod.end(), vec_different_prod_aux.begin(), vec_different_prod_aux.end());
			vec_different_prod_aux.clear();
		}
	}

	Visualize::instance()->printRecommended(mostBought(vec_different_prod));
}

//====================================================================================
 //================================= MISCELLANEOUS ====================================
 //====================================================================================

unsigned int Supermarket::Trans::getBiggestID() const
{
	unsigned int max = 0;
	for (Trans_t T : info_trans)
	{
		if (T.number > max)
			max = T.number;
	}
	return max;
}

void Supermarket::Trans::update()
{
	ofstream fout;
	fout.open(temp_file_name);
	if (fout.is_open())
		fout << info_trans.size();
		for (Trans_t i : this->info_trans)																									//create function to return string of products to display
			fout << i.number << " ; " << to_string(i.date.day) + "/" + to_string(i.date.month) + "/" + to_string(i.date.year) << " ; " << Visualize::instance()->P_comma(i.products) << endl;
	fout.close();
	remove(this->trans_file_name.c_str());
	rename(temp_file_name.c_str(), this->trans_file_name.c_str());
}

vector<string> Supermarket::Trans::askProduct(bool first_time) const
{
	int n_prod;
	bool found = false;
	vector<string> prod_bought;
	map<int, string> num_prod = Visualize::instance()->visAllProd(first_time);
	auto it = num_prod.begin();
	do {
		found = false;
		cin >> n_prod;
		for (it = num_prod.begin(); it != num_prod.end(); it++)
		{
			if (it->first == n_prod)
			{
				found = true;
				break;
			}
		}

		if ((cin.fail() || !found) && !cin.eof())
		{
			cin.clear();
			cin.ignore(9999, '\n');
			cout << "Invalid input, try again" << endl;
			cout << "---> ";
			continue;
		}
		else if (cin.eof())
			break;
		prod_bought.push_back(it->second);
		cout << "---> ";

	} while (!cin.eof());
	return prod_bought;
}

vector<Trans_t> &Supermarket::Trans::getInfo()
{
	return info_trans;
}
