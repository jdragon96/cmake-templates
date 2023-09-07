#ifndef DB_MODEL_SUBDOCUMENT_H
#define DB_MODEL_SUBDOCUMENT_H

#include <mongocxx/client.hpp>
#include <mongocxx/client_session.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/pool.hpp>
#include <mongocxx/stdx.hpp>
#include <mongocxx/uri.hpp>
#include <string>

namespace DB {
namespace Model {

//=> MongoDB collection에 사용되는 인터페이스다.
class SubDocument {
	public:
};

}	 // namespace Model
}	 // namespace DB

#endif