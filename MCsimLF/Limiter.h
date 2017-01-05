#pragma once

namespace MCsimLF {
	namespace Helpers {
		template <class T>
		ref class Limiter abstract
		{
			public:
				static T minCheck(T value, T min);
				static T maxCheck(T value, T max);
				static T minMaxCheck(T value, T min, T max);
		};

		template <class T>
		T Limiter<T>::minCheck(T value, T min)
		{ return (value < min) ? min : value; }

		template <class T>
		T Limiter<T>::maxCheck(T value, T max)
		{ return (value > max) ? max : value; }

		template <class T>
		T Limiter<T>::minMaxCheck(T value, T min, T max)
		{
			return (value < min) ?
					min :
					(value > max) ?
						max :
						value;
		}
	}
}