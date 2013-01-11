#ifndef _LINKED_KAR_HPP_
#define _LINKED_KAR_HPP_

#include <kar.hpp>

namespace Kiss
{
	namespace Project
	{
		class LinkedKar
		{
		public:
			
			static bool addLink(const Kiss::KarPtr &archive, const QString &path);
			static bool removeLink(const Kiss::KarPtr &archive, const QString &path);
			static QStringList links(const Kiss::KarPtr &archive);
			
		private:
			LinkedKar();
		};
	}
}

#endif
