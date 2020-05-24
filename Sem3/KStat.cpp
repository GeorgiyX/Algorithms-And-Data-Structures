partition#include<iostream>
#include<algorithm>

int partition( int* arr, int l, int r )
{
	int pivot = arr[l];
	int i = l + 1;
	int j = r;
	while( true ) {
		while( i <= r && arr[i] < pivot )
			i++;
		while( arr[j] > pivot )
			j--;

		if( i >= j ) {
			std::swap( arr[l], arr[j] );
			return j;
		}
		std::swap( arr[i], arr[j] );
		i++;
		j--;
	}
	return 0;
}

int findKStat( int* arr, int l, int r, int k )
{
	int pivotPos = partition( arr, l, r );
	if( pivotPos == k )
		return arr[pivotPos];
	return k < pivotPos ?
		findKStat( arr, l, pivotPos - 1, k ) : findKStat( arr, pivotPos + 1, r, k );
}

void countSort( int* data, int n )
{
	int minValue = data[0];
	int maxValue = data[0];
	for( int i = 1; i < n; i++ ) {
		minValue = std::min( minValue, data[i] );
		maxValue = std::max( maxValue, data[i] );
	}
	int valuesCount = maxValue - minValue + 1;
	int* c = new int[valuesCount];
	memset( c, 0, valuesCount * sizeof( int ) );
	for( int i = 0; i < n; i++ )
		c[data[i] - minValue]++;

/*	int index = 0;
	for( int i = 0; i < valuesCount; i++ )
		for( int j = 0; j < c[i]; j++ ) {
			data[index] = minValue + i;
			index++;
		}*/

	for( int i = 1; i < valuesCount; i++ )
		c[i] += c[i - 1];

	int* tempData = new int[n];
	for( int i = n - 1; i >= 0; i-- ) {
		int index = data[i] - minValue;
		c[index]--;
		tempData[c[index]] = data[i];
	}

	memcpy( data, tempData, n * sizeof( int ) );
	delete[] c;
	delete[] tempData;
}


int main()
{
	int n = 0;
	std::cin >> n;
	int* arr = new int[n];
	for( int i = 0; i < n; i++ )
		std::cin >> arr[i];
	countSort( arr, n );
	for( int i = 0; i < n; i++ )
		std::cout << arr[i] << " ";
	delete[] arr;
	return 0;
}