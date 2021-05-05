#include <iostream>
#include <vector>
#include <memory>
#include <queue>

std::queue<int> build_balace_queue(int i, int f){
	static std::queue<int> q{};
	if (f == i){
		q.push(i);
	}
	else if (f-i+1 <= 2){
		q.push(i);
		q.push(f);
	}
	else{
		int m = i + (f-i)/2;
		q.push(m);

		build_balace_queue(i, m-1);
		build_balace_queue(m+1, f);
	}
	return q;
}

void print_queue(std::queue<int> q)
{
  while (!q.empty())
  {
    std::cout << q.front() << " ";
    q.pop();
  }
  std::cout << std::endl;
}

int main(){
	auto qu{build_balace_queue(0, 9)};

	print_queue(qu);

}