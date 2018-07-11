#include <vector>
#include "Ticketing.hpp"

namespace Tixico {
    using namespace eosio;
    using std::string;

    class Ticketing : public contract {
        using contract::contract;

    public:
        Ticketing(account_name self):contract(self) {}

        // @abi action
        void addticket(const account_name account, uint64_t id, uint64_t event, uint64_t category, uint64_t cart, uint64_t row, uint64_t seat) {
                require_auth(account);

                ticketIndex tickets(_self, _self);
                auto iterator = tickets.find(id);
                eosio_assert(iterator == tickets.end(), "ticket already exists");
                // TODO: check if ticket is being created by the event/category creator
                // TODO: check if cart, category and event exists
                // TODO: push ticket to cart also
                tickets.emplace(account, [&](auto& _ticket) {
                    _ticket.account_name = account;
                    _ticket.id = id;
                    _ticket.event_id = event;
                    _ticket.category_id = category;
                    _ticket.cart_id = cart;
                    _ticket.row = row;
                    _ticket.seat = seat;
                    _ticket.burned = false;
                });
        }

        // @abi action
        void addevent(const account_name account, uint64_t id, string& title, string& location, uint64_t starts) {
                require_auth(account);

                eventIndex events(_self, _self);
                auto iterator = events.find(id);
                eosio_assert(iterator == events.end(), "event already exists");

                events.emplace(account, [&](auto& _event) {
                    _event.account_name = account;
                    _event.id = id;
                    _event.title = title;
                    _event.location = location;
                    _event.starts = starts;
                });
        }

        // @abi action
        void addcategory(const account_name account, uint64_t id, string& title, uint64_t event, uint64_t price) {
                require_auth(account);

                categoryIndex categories(_self, _self);
                auto iterator = categories.find(id);
                eosio_assert(iterator == categories.end(), "category already exists");
                // TODO: check if category is being created by the event creator

                categories.emplace(account, [&](auto& _category) {
                    _category.account_name = account;
                    _category.id = id;
                    _category.title = title;
                    _category.event_id = event;
                    _category.price = price;
                });

        }

        // @abi action
        void addcart(const account_name account, uint64_t id, string& email) {
                require_auth(account);

                cartIndex carts(_self, _self);
                auto iterator = carts.find(id);
                eosio_assert(iterator == carts.end(), "cart already exists");

                carts.emplace(account, [&](auto& _cart) {
                    _cart.account_name = account;
                    _cart.id = id;
                    _cart.email = email;
                });
        }

        // @abi action
        void burnticket(const account_name account, uint64_t id) {
                require_auth(account);

                ticketIndex tickets(_self, _self);
                auto iterator = tickets.find(id);
                eosio_assert(iterator != tickets.end(), "ticket not found");
                auto ticket = (*iterator);
                eosio_assert(ticket.burned != true, "ticket already burned");
                eosio_assert(ticket.account_name == account, "only creator can burn ticket");

                tickets.modify(iterator, account, [&](auto& tckt) {
                    tckt.burned = true;
                });

        }

        // @abi action
        void removecat(const account_name account, uint64_t id) {
                require_auth(account);

                categoryIndex categories(_self, _self);
                auto iterator = categories.find(id);
                eosio_assert(iterator != categories.end(), "category not found");
                auto category = (*iterator);
                eosio_assert(category.account_name == account, "only creator can remove category");
                categories.erase(iterator);
        }

        // @abi action
        void removeevent(const account_name account, uint64_t id) {
                require_auth(account);

                eventIndex events(_self, _self);
                auto iterator = events.find(id);
                eosio_assert(iterator != events.end(), "category not found");
                auto event = (*iterator);
                eosio_assert(event.account_name == account, "only creator can remove category");
                events.erase(iterator);
        }

        // @abi action
        void getticket(const account_name account, uint64_t id) {
                require_auth(account);

                ticketIndex tickets(_self, _self);
                auto iterator = tickets.find(id);
                eosio_assert(iterator != tickets.end(), "category not found");
                auto ticket = (*iterator);

                print(" id: ", ticket.id,
                      " category: ", ticket.category_id,
                      " event: ", ticket.event_id,
                      " cart: ", ticket.cart_id,
                      " row: ", ticket.row,
                      " seat: ", ticket.seat,
                      " burned: ", ticket.burned
                      );
        }

        // @abi action
        void getcart(const account_name account, uint64_t id) {
                require_auth(account);

                cartIndex carts(_self, _self);
                auto iterator = carts.find(id);
                eosio_assert(iterator != carts.end(), "category not found");
                auto cart = (*iterator);

                print(" id: ", cart.id,
                      " email: ", cart.email
                );
                // TODO: print cart tickets

        }

        // @abi action
        void getcat(const account_name account, uint64_t id) {
                require_auth(account);

                categoryIndex categories(_self, _self);
                auto iterator = categories.find(id);
                eosio_assert(iterator != categories.end(), "category not found");
                auto category = (*iterator);

                print(" id: ", category.id,
                      " event: ", category.event_id,
                      " price: ", category.price,
                      " title: ", category.title
                );

        }

        // @abi action
        void getevent(const account_name account, uint64_t id) {
                require_auth(account);

                eventIndex events(_self, _self);
                auto iterator = events.find(id);
                eosio_assert(iterator != events.end(), "category not found");
                auto event = (*iterator);

                print(" id: ", event.id,
                      " location: ", event.location,
                      " starts: ", event.starts,
                      " title: ", event.title
                );
        }

    private:

        //@abi table ticket i64
        struct ticket {
            uint64_t account_name;
            uint64_t id;
            uint64_t category_id;
            uint64_t event_id;
            uint64_t cart_id;
            uint64_t row;
            uint64_t seat;
            bool burned;

            uint64_t primary_key() const { return id; }

            EOSLIB_SERIALIZE(ticket, (account_name)(id)(category_id)(event_id)(cart_id)(row)(seat)(burned))
        };

        typedef multi_index<N(ticket), ticket> ticketIndex;

        //@abi table category i64
        struct category {
            uint64_t account_name;
            uint64_t id;
            string title;
            uint64_t event_id;
            uint64_t price;

            uint64_t primary_key() const { return id; }

            EOSLIB_SERIALIZE(category, (account_name)(id)(title)(event_id)(price))
        };

        typedef multi_index<N(category), category> categoryIndex;

        //@abi table event i64
        struct event {
            uint64_t account_name;
            uint64_t id;
            string title;
            string location;
            uint64_t starts; // unix timestamp

            uint64_t primary_key() const { return id; }

            EOSLIB_SERIALIZE(event, (account_name)(id)(title)(location)(starts))
        };

        typedef multi_index<N(event), event> eventIndex;

        //@abi table cart i64
        struct cart {
            uint64_t account_name;
            uint64_t id;
            string email;
            // TODO: keep tickets in cart
            // vector <uint64_t> tickets; // max up to 5 tickets per cart currently

            uint64_t primary_key() const { return id; }

            EOSLIB_SERIALIZE(cart, (account_name)(id)(email))
        };

        typedef multi_index<N(cart), cart> cartIndex;
    };

    EOSIO_ABI(Ticketing, (addticket)(addevent)(addcategory)(addcart)(burnticket)(removecat)(removeevent)(getticket)(getcart)(getcat)(getevent))
}
