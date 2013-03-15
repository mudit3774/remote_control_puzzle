#include<iostream>
#include<vector>
#include<set>

#define MIN 0 
#define MAX 10000
#define MAX_BLOCKED 40
#define MAX_VIEW 50

using namespace std;

//Class to mimic the behaviour of the the remote control
class Remote
{
  	//Set (to ignore duplicates) of blocked channels 
		set<int> blocked;

		//Wishlist of channels user wishes to watch
		vector<int> wish;

		//Starting channel, ending channel, number of channels blocked, number of channels viewed
		int start,end,num_blocked,num_view;

		//Check validity of input according to the problem statement
		bool is_valid_ch(int);
		bool is_valid_st(int);
		bool is_valid_en(int,int);
		bool is_valid_lim(int,int);
		bool is_valid_blocked_num(int);
		bool is_valid_view_num(int);		
		
		//Check accessibility of channels 
		bool is_viewable(int);
		bool is_blocked(int);
		
		//Calculate number of clicks required by various possible combinations
		int num_fwdbkswap(int,int,int);
		int num_fwdbk(int,int);
		int num_direct(int);
		
		//Takes input
		bool get_input();
	
		//Calculates number of clicks
		int num_clicks();

		//Other helper function	
		int fmin(int,int,int);
	public:
		//To run the feature described in the problem statement
		int run();
};

int Remote::run(void)
{
	if(!get_input())
	{
		cout<<"Invalid Input";
		return -1;
	}
	return num_clicks();
}
bool Remote::is_valid_blocked_num(int b)
{
	if(b>MAX_BLOCKED||b<0)
	{
		cout<<"Invalid number of blocked channels";		
		return false;
	}
	return true;
}

bool Remote::is_valid_view_num(int b)
{
	if(b>MAX_VIEW||b<0)
	{
		cout<<"Invalid number of channels to be viewed";		
		return false;
	}
	return true;
}
bool Remote::get_input()
{
	int tmp;
	cin>>start>>end;
	if(!is_valid_lim(start, end))
		return false;
	cin>>num_blocked;
	if(!is_valid_blocked_num(num_blocked))
		return false;
	for(int i =0;i<num_blocked;i++)
	{
		cin>>tmp;
		if(!is_valid_ch(tmp))
		{
			cout<<"Invalid channel number";
			return false;
		}
		blocked.insert(tmp);
	}
	cin>>num_view;
	if(!is_valid_view_num(num_view))
		return false;
	for(int i =0;i<num_view;i++)
	{
		cin>>tmp;
		if(!is_valid_ch(tmp))
		{
			cout<<"Invalid channel number";
			return false;
		}
		wish.push_back(tmp);
	}
	return true;
}


int Remote::num_direct(int ch)
{
	int count=0;
	do
	{
		ch = ch/10;
		count++;
	}
	while(ch!=0);
	return count;
}

int Remote::num_fwdbk(int cur,int wnt)
{
	int count=0;
	int st,en,fwd,bk;
	if(cur<wnt)	
	{
		st = cur;
		en = wnt;
	}
	else
	{	
		st = wnt;
		en = cur;
	}
	bk = (st-start) + (end-en);
	fwd = en-st;
	if(fwd<bk)
	{
		for(int i=st;i<en;i++)
			if(!is_blocked(i))	
				count++;
	}
	else
	{
		for(int i=st;i>start;i--)
			if(!is_blocked(i))	
				count++;
		for(int i=end;i>=en;i--)
			if(!is_blocked(i))	
				count++;
		
	}
	return count;
}

int Remote::fmin(int a,int b,int c)
{
	if(a<b)
	{
		if(a<c)
			return a;
		return c;
	}
	else if(b<c)
		return b;
	return c;
}

int Remote::num_fwdbkswap(int l, int curr, int nxt)
{
	return (num_fwdbk(l,nxt) + 1);
}
int Remote::num_clicks(void)
{
	int count=0;
	vector<int>::iterator i;	
	int last,current,next;
	count = num_direct(wish.at(0));
	last=-1;
	for(i=wish.begin();(i+1)!=wish.end();++i)	
	{
		if(i!=wish.begin())
			last = *(i-1);
		current = *i;
		next = *(i+1);
		if(next==current)
			continue;
		if(last==next)
		{
			count++;
			continue;
		}
		count += fmin(num_fwdbk(current,next),num_direct(next),num_fwdbkswap(last,current,next));
	}
	return count;
}

bool Remote::is_blocked(int ch)
{
	if(blocked.find(ch)!=blocked.end())
		return true;
	return false;
		
}

bool Remote::is_viewable(int ch)
{
	if(!is_blocked(ch)&&is_valid_ch(ch))
		return true;
	return false;
}

bool Remote::is_valid_lim(int st, int en)
{
	if(!is_valid_st(st))
	{
		cout<<"Channel : "<<st<<";"<<"Invalid Starting Channel Number";
		return false;
	}
	else if(!is_valid_en(st,en))
	{	
		cout<<"Channel : "<<en<<";"<<"Invalid Last Channel Number";
		return false;
	}
	return true;
}

bool Remote::is_valid_en(int st, int en)
{
	if(en>MAX||en<st)
		return false;
	return true;
}
bool Remote::is_valid_st(int st)
{
	if(st<MIN||st>MAX)
		return false;
	return true;
}

bool Remote::is_valid_ch(int ch)
{
	if(ch<start||ch>end)		
 		return false;
	return true;	
}

int main()
{
	Remote obj;
	cout<<obj.run()<<endl;
}
