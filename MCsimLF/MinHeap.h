#pragma once

namespace MCsimLF {
	namespace FileManip {
		using namespace System;
		using namespace System::Collections::Generic;

		template <class T>
		public ref class MinHeap
		{
			public:
				MinHeap();
				MinHeap(UInt32 capacity);
				
				bool isEmpty();
				Int32 getSize();

				void insert(T item);
				T extract();

				array<T>^ toArray();

			private:
				List<T>^ _data;

				void _fixUp(UInt32 iToFix);
				void _fixDown(UInt32 iToFix);

				UInt32 _getParentIndex(UInt32 iItem);
				bool _isParentGreater(UInt32 iItem);

				UInt32 _getLeftChildIndex(UInt32 iItem);
				UInt32 _getRightChildIndex(UInt32 iItem);
				bool _hasLeftChild(UInt32 iItem);
				bool _hasRightChild(UInt32 iItem);
				bool _isLeftChildGreater(UInt32 iItem);
				bool _isRightChildGreater(UInt32 iItem);
		};

		template <class T>
		MinHeap<T>::MinHeap()
		{
			_data = gcnew List<T>(0);
		}

		template <class T>
		MinHeap<T>::MinHeap(UInt32 capacity)
		{
			_data = gcnew List<T>(0);
			_data->Capacity = capacity;
		}
		
		template <class T>
		bool MinHeap<T>::isEmpty()
		{
			return (_data->Count == 0);
		}

		template <class T>
		Int32 MinHeap<T>::getSize()
		{
			return _data->Count;
		}

		template <class T>
		void MinHeap<T>::insert(T item)
		{
			_data->Add(item);
			_fixUp(_data->Count - 1);
		}

		template <class T>
		T MinHeap<T>::extract()
		{
			T min = _data[0];
			_data[0] = _data[_data->Count - 1];
			_data->RemoveAt(_data->Count - 1);

			_fixDown(0);
			return min;
		}

		template <class T>
		array<T>^ MinHeap<T>::toArray()
		{
			array<T>^ output = gcnew array<T>(_data->Count);
			for(Int32 i = 0; i < _data->Count; ++i)
				output[i] = _data[i];
			return output;
		}

		template <class T>
		void MinHeap<T>::_fixUp(UInt32 iToFix)
		{
			while((iToFix > 0) && _isParentGreater(iToFix)) {
				UInt32 iNew = _getParentIndex(iToFix);

				T tmp = _data[iToFix];
				_data[iToFix] = _data[iNew];
				_data[iNew] = tmp;

				iToFix = iNew;
			}
		}

		template <class T>
		void MinHeap<T>::_fixDown(UInt32 iToFix)
		{
			UInt32 iMin;
			while(_hasLeftChild(iToFix)) {
				iMin = iToFix;
				if(_data[iMin] > _data[_getLeftChildIndex(iToFix)])
					iMin = _getLeftChildIndex(iToFix);

				if(_hasRightChild(iToFix) && (_data[iMin] > _data[_getRightChildIndex(iToFix)]))
					iMin = _getRightChildIndex(iToFix);

				if(iMin != iToFix) {
					T tmp = _data[iToFix];
					_data[iToFix] = _data[iMin];
					_data[iMin] = tmp;

					iToFix = iMin;
				}
				else break;
			}
		}

		template <class T>
		UInt32 MinHeap<T>::_getParentIndex(UInt32 iItem)
		{
			return (iItem > 0) ? (iItem - 1) >> 1 : 0;
		}

		template <class T>
		bool MinHeap<T>::_isParentGreater(UInt32 iItem)
		{
			return (_data[_getParentIndex(iItem)] > _data[iItem]);
		}

		template <class T>
		UInt32 MinHeap<T>::_getLeftChildIndex(UInt32 iItem)
		{
			return _getRightChildIndex(iItem) - 1;
		}

		template <class T>
		UInt32 MinHeap<T>::_getRightChildIndex(UInt32 iItem)
		{
			return (iItem + 1) << 1;
		}

		template <class T>
		bool MinHeap<T>::_hasLeftChild(UInt32 iItem)
		{
			return (_getLeftChildIndex(iItem) < (UInt32)_data->Count);
		}

		template <class T>
		bool MinHeap<T>::_hasRightChild(UInt32 iItem)
		{
			return (_getRightChildIndex(iItem) < (UInt32)_data->Count);
		}

		template <class T>
		bool MinHeap<T>::_isLeftChildGreater(UInt32 iItem)
		{
			return (_data[_getLeftChildIndex(iItem)] > _data[iItem]);
		}

		template <class T>
		bool MinHeap<T>::_isRightChildGreater(UInt32 iItem)
		{
			return (_data[_getRightChildIndex(iItem)] > _data[iItem]);
		}
	}
}