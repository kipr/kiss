#ifndef _LINKED_KAR_HPP_
#define _LINKED_KAR_HPP_

#include <kar.hpp>

namespace kiss
{
	namespace project
	{
		class LinkedKar
		{
		public:
			
			static bool addLink(const kiss::KarPtr &archive, const QString &path);
			static bool removeLink(const kiss::KarPtr &archive, const QString &path);
			static QStringList links(const kiss::KarPtr &archive);
			
		private:
			LinkedKar();
		};
	}
}

#endif
