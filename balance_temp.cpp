//					###balance();###
	void build_balace_queue(int i, int f, std::queue<Tkey>* q){
		// warning, static may stay the same for different trees!
		//std::queue<int> q{};
		if (f == i){
			q->push(i);
		}
		else if (f-i+1 == 2){
			q->push(i);
			q->push(f);
		}
		else{
			int m = i + ceil((float)(f-i)/2);
			q->push(m);

			build_balace_queue(i, m-1, q);
			build_balace_queue(m+1, f, q);
		}
		return;
	}

	void balance(){
		// save all tree content in an array
		std::vector<std::pair<Tkey, Tvalue>> t_content{};
		for (auto x:*this){
			t_content.push_back(x);
		}
		//std::cout << n << std::endl;
		//std::cout << t_content[1].second << std::endl;

		// build a queque for insertion
		std::queue<Tkey> queue;
		build_balace_queue(0, t_content.size()-1, &queue);

		// build a balaced tree
		BST<Tkey, Tvalue> tmp_t{};

		while (!queue.empty())
		{
			// queue.front() is the key of the last element in the stack
			// t_content[queue.front()] is the pair <key, value>
		   	tmp_t.insert(t_content[queue.front()]);
		   	std::cout << queue.front() << " " ;
		   	queue.pop();
		}
		std::cout << std::endl;

		// swap balanced tree and this

		*this = std::move(tmp_t);
	}