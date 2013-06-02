#include<iostream>
#include<vector>
#include<climits>
#include<algorithm>
#include "cities.h"
using namespace std;

class Tour {
 public:
  //used to initialize new tours.
  Tour(){ cities.push_back(0); mileage = 0; }
  
  //used to initialize best tour. set to a high
  //cost so that the first complete tour will become
  //the new best.
  Tour(unsigned int cost){ mileage = cost; }
  
  
  //copy constructor -- in recursive algorithm,
  //only used to update the best tour
  Tour(const Tour& t) { //usually const Tour& t
    cities = t.cities;
    mileage = t.mileage;
  }

  unsigned int get_num_cities() {return cities.size(); } 
  
  //returns true if city c is in the tour
  bool is_present(unsigned int c){
    return (find(cities.begin(), cities.end(), c) != cities.end());
  }
  
  //returns true if this tour is better than tour t
  bool better_than(const Tour& t){ return (mileage < t.mileage); }
  
  void set_tour(Tour t){
    cities = t.cities;
    mileage = t.mileage;
  }

  void print_best(){
    cout<<"the best mileage is: "<<this->mileage<<endl;
  }
  
  //----Adding and removing citites----
  
  //compute change in mileage if city c is added to the list 
  unsigned int mileage_delta(unsigned int c) {
    unsigned int last = cities.back(); //current last city
    unsigned int test_mileage = mileage_table[c][0];
    
    //if this would be our last city, add the mileage to return home
    if(cities.size() + 1 == num_cities){
      test_mileage += mileage_table[c][0];
    }
    return test_mileage;
  }
  
  //add city unconditionally (without checking anything)
  void add_city(unsigned int c){
    mileage += mileage_delta(c);
    cities.push_back(c);
  }
  
  //add a city conditionally, check to make sure that (a) it is not already present
  //and (b) it would not lead to a tour that is worse than the best tour so far
  //return true if city was added
  bool add_city(unsigned int c, const Tour& best_so_far) {
    //if city is already in the list, return fasle
    if(is_present(c)){
      return false;
    }
    //if this would be worse than our best completed tour, return false
    if(mileage + mileage_delta(c) > best_so_far.mileage) {
#if DEBUG
      cout<< "not adding " <<city_names[c]
	  <<" -- mileage would be " <<mileage + mileage_delta(c)
	  <<" -- best is " << best_so_far.mileage << endl;
#endif
      return false;
    }
    add_city(c);
#if DEBUG
    cout<<"Added " << city_names[c] <<endl;
    cout<<"tour is now: " << *this;
#endif
    return true;
  }
  
  //delete the last city
  void remove_last() {
    unsigned int c = cities.back();
    cities.pop_back();
    
    //subtract the mileage that c added to the cost
    mileage -= mileage_delta(c);
#if DEBUG
    cout<<"removed "<< city_names[c] <<endl;
    cout<<"tour is now: "<<*this;
#endif
  }
  
  //output operator: print cost and list of names
  friend ostream& operator << (ostream &outs, Tour t) {
    cout<<"\ncost = " << t.mileage <<endl;
    for (vector<unsigned int>::iterator it = t.cities.begin(); it != t.cities.end(); it++){
      cout << city_names[*it] <<endl;
    }
    cout<<endl;
    return outs;
  }
  
  
 private:
  vector<unsigned int>cities;
  unsigned int mileage;
  
};
