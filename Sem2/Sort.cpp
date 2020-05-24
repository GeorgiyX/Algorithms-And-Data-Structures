#include <iostream>

struct Point {
	int X;
	int Y;
	Point() : X( 0 ), Y( 0 ) {}
};

std::ostream& operator << ( std::ostream& os, const Point& pt )
{
	return os << "(" << pt.X << ", " << pt.Y << ")";
}

// 1
bool operator < ( const Point& l, const Point& r )
{
	return l.X < r.X;
}

// 2
bool IsLess234ByX ( const Point& l, const Point& r )
{
	return l.X < r.X;
}

bool IsLess432ByY ( const Point& l, const Point& r )
{
	return l.Y < r.Y;
}

// 3
template<class T>
bool IsLessDefault( const T& l, const T& r )
{
	return l < r;
}

// 4
class IsLessBy3rdPoint {
public:
	IsLessBy3rdPoint( const Point _pt ) : pt( _pt ) {}

	bool operator()(const Point& l, const Point& r)
	{
		return (l.X - pt.X) * (l.X - pt.X) + (l.Y - pt.Y) * (l.Y - pt.Y) < (r.X - pt.X) * (r.X - pt.X) + (r.Y - pt.Y) * (r.Y - pt.Y);
	}

private:
	Point pt;
};

// 5
template<class T>
class IsLessDefaultFunctor {
public:
	bool operator()(const T& l, const T& r)
	{
		return l < r;
	}
};

template<class T, class Compare = IsLessDefaultFunctor<T> >
void mySort( T* arr, int l, int r, Compare isLess = Compare() )
{
	for( int i = l; i < r; i++ )
		for( int j = l; j < r - 1; j++ )
			if( isLess( arr[j + 1], arr[j] ) )
				std::swap( arr[j], arr[j + 1] );
}

int main()
{
	int n = 0;
	std::cin >> n;
	Point* arr = new Point[n];
	for( int i = 0; i < n; i++ )
		std::cin >> arr[i].X >> arr[i].Y;


	mySort( arr, 0, n );
	mySort( arr, 0, n, IsLess234ByX );
	mySort( arr, 1, n, IsLessBy3rdPoint(arr[0]) );
	mySort( arr, 0, n, []( const Point& l, const Point& r ) {return l.Y < r.Y; } );

	for( int i = 0; i < n; i++ )
		std::cout << arr[i] << " ";

	delete[] arr;
	return 0;
}
