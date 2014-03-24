#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <bitset>
#include <stdexcept>

using namespace std;

struct run {
	unsigned	size;			// number of addends
	unsigned	target_sum;	// actual sum for reference
	bitset<10> 	set;			// addends in the set

	run(unsigned target_sum, unsigned size) :
		size(size), target_sum(target_sum)
	{ }
	run(unsigned s);

	void add(int n);
};


run::run(unsigned s) : target_sum(0), set(s)
{
	for (s = 1; s < 10; s++)
		if (set[s])
			target_sum += s;
	size = set.count();
}

void run::add(int n)
{
	if (n > 0) {
		if (set[n])
			throw logic_error("run constraints violated");
		set[n] = true;
	}
	else
		set[-n] = false;
}


static vector<vector<run> > G_sum_table;

//Sets up the board for when we run the main algo
static void generate_sum_table()
{
	/* The maximum sum is 45, minimum 0. */
	G_sum_table.resize(46);
	for (unsigned i = 0; i < 512; i++) {
		/* Shift left since 0 is never used in sums. */
		run r(i << 1);
		G_sum_table[r.target_sum].push_back(r);
	}
}

//The actual grid that holds the sums down and right, as well as the solution
struct grid {
	//Just a simple def for down and right
	enum Direction {
		DOWN = 0, RIGHT
	};

	struct run_pair {
		run *r_[2];

		run_pair(run *d = 0, run *r = 0)
		{
			r_[0] = d; r_[1] = r;
		}
	};

	unsigned		rows_, cols_;	
	//the solution to the puzzel is stored in this array
	vector<unsigned> cells_;		
	vector<run_pair> runs_;			

	grid() : rows_(0), cols_(0)
	{ }

	void setsize(unsigned rows, unsigned cols);
	bool put(unsigned idx, unsigned n);
	bool add_run(unsigned startidx, unsigned target_sum, unsigned size, Direction dir);
};

//Want to set the size of the board 
void grid::setsize(unsigned rows, unsigned cols)
{
	//Just creat a local variable for the resize
	rows_ = rows; cols_ = cols;

	//Use the built in vector function to do the resizing
	cells_.resize(rows_ * cols_, -2U);
	runs_.resize(rows_ * cols_);
}


//given a location idx and a solution we add the solution to the board cell
bool grid::put(unsigned idx, unsigned n)
{
	unsigned oldn = cells_[idx];

	//The puzzel only lets for value less then 9, should never be true but just in case...
	if (n > 9)
		throw invalid_argument("invalid argument to grid::put()");

	if (runs_[idx].r_[0]->set[n] || runs_[idx].r_[1]->set[n])
		return false;

	runs_[idx].r_[0]->add(-oldn);
	runs_[idx].r_[1]->add(-oldn);

	runs_[idx].r_[0]->add(n);
	runs_[idx].r_[1]->add(n);
	cells_[idx] = n;

	return true;
}


bool grid::add_run(
	unsigned startidx,
	unsigned target_sum,
	unsigned size,
	Direction dir)
{
	unsigned incr = dir == RIGHT ? 1 : cols_;

	if ((dir != DOWN) && (dir != RIGHT))
		throw invalid_argument("invalid arguments to grid::add_run()");

	if (!cells_[startidx])
		return false;

	cells_[startidx] = -1U;

	run *r = new run(target_sum, size);
	for (startidx += incr; size; startidx += incr, size--) {

		if (startidx >= rows_ * cols_)
			return false;

		if (cells_[startidx] == -1U)
			return false;

		if (runs_[startidx].r_[dir])
			return false;

		runs_[startidx].r_[dir] = r;
		cells_[startidx] = 0;
	}

	return true;
}

//read data from the file
static bool read_data(ifstream &is, grid &b)
{
	unsigned idx;
	string line;

	if (!getline(is, line)) {
		return false;
	}
	else {
		unsigned rows, cols;
		istringstream iss(line);

		if (!(iss >> rows >> cols)) {
			return false;
		}

		if ((rows < 2) || (cols < 2)) {
			return false;
		}

		b.setsize(rows, cols);
	}

	if (!getline(is, line)) {
		return false;
	}
	else {
		istringstream iss(line);

		while (iss >> idx) {
			b.cells_[idx] = -1U;
		}
		if (!iss.eof()) {
			return false;
		}
	}

	while (getline(is, line)) {
		unsigned sum[2], size[2];
		istringstream iss(line);

		if (!(iss >> idx >> sum[0] >> size[0] >> sum[1] >> size[1])) {
			return false;
		}
		if ((sum[0] > 45) || (sum[1] > 45) || (size[0] > 9) || (size[1] > 9)) {
			return false;
		}
		if (!b.add_run(idx, sum[0], size[0], grid::DOWN)) {
			return false;
		}
		if (!b.add_run(idx, sum[1], size[1], grid::RIGHT)) {
			return false;
		}
	}

	for (idx = 0; idx < b.cells_.size(); idx++) {
		if (b.cells_[idx] == -2U) {
			return false;
		}

		if ((b.cells_[idx] != 0) && (b.cells_[idx] != -1U))
			throw logic_error("invalid grid initialization");

		if ((b.cells_[idx] == 0)
			&& (!b.runs_[idx].r_[0] || !b.runs_[idx].r_[1])) {
			return false;
		}
	}

	return true;
}

//print the board
static void print(const grid &b)
{
	for (unsigned i = 0; i < b.rows_ * b.cols_; i++) {
		if (i % b.cols_ == 0)
			cout << endl;
		if (b.cells_[i] == -1U)
			cout << '.';
		else
			cout << b.cells_[i];
	}
	cout << endl << endl;
}

static bitset<10> feasible_addend_set(const run *r)
{
	bitset<10> result;

	if (r->target_sum > 45)
		throw invalid_argument("run inconsistency detected in feasible_addend_set()");


	for (unsigned i = 0; i < G_sum_table[r->target_sum].size(); i++) {
		run ns = G_sum_table[r->target_sum][i];

		if ((ns.size == r->size) && ((ns.set & r->set) == r->set))
			result |= ns.set;
	}

	return result & ~r->set;
}

//checks if the value that we came up with are sane
static bitset<10> feasible_numbers(const grid &b, unsigned idx)
{
	bitset<10> result_down, result_right;

	if (b.runs_[idx].r_[0])
		result_down = feasible_addend_set(b.runs_[idx].r_[0]);
	if (b.runs_[idx].r_[1])
		result_right = feasible_addend_set(b.runs_[idx].r_[1]);

	return result_down & result_right;
}

//the main solze method
static void solve(grid &b, unsigned idx)
{
	if (idx >= b.rows_ * b.cols_) {
		print(b);
		return;
	}

	if (b.cells_[idx] == -1U) {
		solve(b, idx + 1);
		return;
	}

	bitset<10> trial_numbers = feasible_numbers(b, idx);
	_Cilk_for(unsigned i = 1; i < 10; i++)
		if (trial_numbers[i]) {
			if (!b.put(idx, i))
				throw logic_error("duplicate trial number");
			solve(b, idx + 1);
			_Cilk_sync;
		}
	b.put(idx, 0);
}

int main(int argc, char **argv) try {
	grid b;

	if (argc != 2) {
		return 1;
	}

	generate_sum_table();

	ifstream is(argv[1]);
	if (!is) {
		return 1;
	}

	if (!read_data(is, b))
		return 1;

	solve(b, 0);
	return 0;
}
catch (exception &e) {
	cerr << "INTERNAL ERROR: " << e.what() << endl;
	return 1;
}
