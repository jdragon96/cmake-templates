#ifndef ROBOTDB_RESOURCE_MONGOMANAGER_H
#define ROBOTDB_RESOURCE_MONGOMANAGER_H

#include <iostream>
#include <map>
#include <mongocxx/client.hpp>
#include <mongocxx/client_session.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/logger.hpp>
#include <mongocxx/pool.hpp>
#include <mongocxx/stdx.hpp>
#include <mongocxx/uri.hpp>
#include <sstream>
#include <string>

namespace DB {
namespace Resource {

struct MongoParameter {
		bool initialized;

		//! IP 주소
		std::string ip;

		//! MongoDB 포트
		int port;

		//! 최소 풀 사이즈
		int minPoolSize;

		//! 최대 풀 사이즈
		int maxPoolSize;
		int a;

		//! 생성자
		MongoParameter()
				: initialized(false),
					ip("localhost"),
					port(27017),
					minPoolSize(3),
					maxPoolSize(10) {}
};

class MongoManager {
	public:
		using connection = mongocxx::pool::entry;

	public:
		//! MongDB 커넥션 시 사용된 변수
		MongoParameter m_params;

		//! 커넥션 스트링
		std::string m_connString;

	private:
		//! collection instance pool...
		std::unique_ptr<mongocxx::instance> _instance = nullptr;
		std::unique_ptr<mongocxx::pool> _pool = nullptr;

	public:
		MongoManager() = default;

	public:
		//! 싱글톤
		static MongoManager& instance() {
			static MongoManager instance;
			return instance;
		}

		//! 커넥션 스트링을 생성한다.
		bool setConnString(MongoParameter& param) {
			this->m_params = param;
			this->m_params.initialized = true;

			std::stringstream stream;
			stream << "mongodb://" << param.ip;
			stream << ":" << param.port;
			stream << "/?minPoolSize=" << param.minPoolSize;
			stream << "&maxPoolSize=" << param.maxPoolSize;
			this->m_connString = stream.str();

			std::cout << this->m_connString << std::endl;

			return this->m_params.initialized;
		}

		//! MongoDB 인스턴스 빌드
		bool build() {
			if (!this->m_params.initialized) return false;

			//=> 로그 수준을 설정한다.
			class noop_logger : public mongocxx::logger {
				public:
					virtual void operator()(mongocxx::log_level,
																	bsoncxx::stdx::string_view,
																	bsoncxx::stdx::string_view) noexcept {}
			};

			try {
				//=> 커넥션 스트링 생성
				mongocxx::uri Uri(this->m_connString);
				//=> MongoDB 인스턴스를 생성한다.
				auto instance = bsoncxx::stdx::make_unique<mongocxx::instance>(bsoncxx::stdx::make_unique<noop_logger>());
				//=> 각 인스턴스들을 보관한다.
				MongoManager::instance().configure(std::move(instance),
																					 bsoncxx::stdx::make_unique<mongocxx::pool>(std::move(Uri)));
				return true;
			} catch (int exception) {
				return false;
			}
		}

		//! MongoDB 커넥션을 생성한다.
		connection get_connection(bool& flag) {
			auto conn = _pool->acquire();
			if (!conn) flag = false;
			flag = true;
			return conn;
		}

	private:
		//! DB config 잡기
		void configure(std::unique_ptr<mongocxx::instance> instance,
									 std::unique_ptr<mongocxx::pool> pool) {
			_instance = std::move(instance);
			_pool = std::move(pool);
		}

	private:
};
}	 // namespace Resource
}	 // namespace RobotDB

#endif