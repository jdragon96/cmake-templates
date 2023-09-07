#ifndef DB_MODEL_DOCUMENT_H
#define DB_MODEL_DOCUMENT_H

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
class Document {
	public:
		//! MongoDB DB 이름
		std::string m_dbName;

		//! MongoDB Collection 이름
		std::string m_collectionName;

	public:
};

}	 // namespace Model
}	 // namespace DB

#endif