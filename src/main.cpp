#include "Cache.h"
#include "Server.h"
#include <memory>

int main()
{
    auto cache = std::make_shared<Cache>();
    Server server(cache, 8080);
    server.Start();
    return 0;
}