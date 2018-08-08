#include <eosiolib/eosio.hpp>
#include <eosiolib/print.hpp>
#include <string>
#include <vector>

namespace Tixico {
    using namespace eosio;
    using std::string;

    class Ticketing : public contract {
        using contract::contract;

    public:
        Ticketing(account_name self):contract(self) {}
        void addowner(const account_name account, uint64_t id);
        // @abi action
        void addticket(const account_name account, uint64_t id, uint64_t owner, uint64_t category);
        // @abi action
        void burnticket(const account_name account, uint64_t id);
        // @abi action
        void getticket(const account_name account, uint64_t id);
        // @abi action
        void gettickets(const account_name account, uint64_t owner_id);
        // @abi action
        void swapowners(const account_name account, uint64_t t_id, uint64_t new_owner_id, uint64_t old_owner_id);
        // @abi action
        void delticket(const account_name account, uint64_t id);

        //@abi table ticket i64
        struct ticket {
            uint64_t account_name;
            uint64_t id;
            uint64_t category_id;
            uint64_t owner_id;
            bool burned;

            uint64_t primary_key() const { return id; }

            EOSLIB_SERIALIZE(ticket, (account_name)(id)(category_id)(owner_id)(burned))
        };

        typedef multi_index<N(ticket), ticket> ticketIndex;

        struct owner {
            uint64_t account_name;
            uint64_t id;
            vector<uint64_t> tickets;

            uint64_t primary_key() const { return id; }

            EOSLIB_SERIALIZE(owner, (account_name)(id)(tickets))
        };

        typedef multi_index<N(owner), owner> ownerIndex;
    };

    EOSIO_ABI(Ticketing, (addticket)(burnticket)(getticket)(gettickets)(swapowners)(delticket))
} // namespace Tixico
