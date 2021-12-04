#include<iostream>

template <typename T>
class Grid {
private:
    T value;
    size_t x_size, y_size;
    Grid *memory;
public:
    Grid(): Grid(0, 0) { }

    Grid(size_t x_size, size_t y_size): value(T()), x_size(x_size), y_size(y_size) {
        if(x_size==0) memory=nullptr; else memory = new Grid[x_size*y_size];
    }

    ~Grid() { delete[] memory; }

    Grid(Grid const &src): Grid(src.x_size, src.y_size) {
        value = src.value;
        if(src.x_size!=0) for(size_t pos=0; pos!=x_size*y_size; ++pos) { memory[pos] = src.memory[pos]; }
    }

    Grid& operator=(Grid const &src) {
        Grid tmp(src);
        std::swap(this->value, tmp.value);
        std::swap(this->memory, tmp.memory);
        std::swap(this->x_size, tmp.x_size);
        std::swap(this->y_size, tmp.y_size);
        return *this;
    }

    Grid(Grid &&src): Grid(src.x_size, src.y_size) {
        value = src.value;
        memory = src.memory;
        src.value = T();
        src.x_size = 0;
        src.y_size = 0;
        src.memory = nullptr;
    }

    Grid& operator=(Grid &&src) {
        Grid tmp(std::move(src));
        std::swap(this->value, tmp.value);
        std::swap(this->memory, tmp.memory);
        std::swap(this->x_size, tmp.x_size);
        std::swap(this->y_size, tmp.y_size);
        return *this;
    }

    T operator()(size_t x_idx, size_t y_idx) const {
        if((memory[y_idx*x_size+x_idx]).is_subgrid()) {
            T sum = T();
            size_t pos = y_idx*x_size+x_idx;
            size_t sub_size = (memory[pos]).x_size*(memory[pos]).y_size;
            for(size_t sub_pos=0;sub_pos!=sub_size;++sub_pos) sum+=((memory[pos]).memory[sub_pos]).value;
            return sum/sub_size;
        }
        else return (memory[y_idx*x_size+x_idx]).value;
    }

    Grid& operator()(size_t x_idx, size_t y_idx) { return memory[y_idx*x_size+x_idx]; }

    size_t get_xsize() const { return x_size; }
    size_t get_ysize() const { return y_size; }

    Grid& operator=(T val) {
        if(x_size==0) value=val;
        else for(size_t pos=0; pos!=x_size*y_size; ++pos) memory[pos] = val;
        return *this;
    }

    Grid& get_subgrid(size_t x_idx, size_t y_idx) { return memory[y_idx*x_size+x_idx]; }
    Grid& get_subgrid(size_t x_idx, size_t y_idx) const { return memory[y_idx*x_size+x_idx]; }

    bool is_subgrid(size_t x_idx, size_t y_idx) const { return (memory[y_idx*x_size+x_idx]).x_size!=0; }

    Grid& collapse_subgrid(size_t x_idx, size_t y_idx) {
        if(this.is_subgrid(x_idx, y_idx)) this.make_subgrid(x_idx, y_idx, 0, 0);
        return *this;
    }

    Grid& make_subgrid(size_t x_idx, size_t y_idx, size_t x_sub_idx, size_t y_sub_idx) {
        Grid tmp(x_sub_idx, y_sub_idx);
        T new_value = this(x_idx, y_idx);
        tmp = new_value;
        std::swap((memory[y_idx*x_size+x_idx]).value, tmp.value);
        std::swap((memory[y_idx*x_size+x_idx]).x_size, tmp.x_size);
        std::swap((memory[y_idx*x_size+x_idx]).y_size, tmp.y_size);
        std::swap((memory[y_idx*x_size+x_idx]).memory, tmp.memory);
        return *this;
    }

    friend std::ostream& operator<<(std::ostream &out, const Grid &grid) {
        if(grid.x_size==0) out<<grid.value<<" ";
        else { out<<"\nGrid:\n"; for(size_t pos=0;pos!=grid.x_size*grid.y_size;++pos) out<<grid.memory[pos]; out<<"\nEnd of grid\n"; }
        return out;
    }

    friend std::istream& operator>>(std::istream &in, Grid &grid) {
        if(grid.x_size==0) in>>grid.value;
        else for(size_t pos=0; pos!=grid.x_size*grid.y_size; ++pos) in>>grid.memory[pos];
        return in;
    }
};

int main(int argc, const char* argv[]) {
    float new_val;
    std::cin>>new_val;
    Grid<float> first_grid(3,4);
    std::cin>>first_grid;
    std::cout<<first_grid<<"\n";

    first_grid = new_val;
    {
        Grid<float> second_grid(first_grid);
        second_grid(1,1) = -3.2;
        std::cout<<second_grid;
    }

    std::cout<<first_grid;

    return 0;
}
