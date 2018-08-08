#include "Ticketing.hpp"

namespace Tixico {
        // @abi action
        void Ticketing::addowner(const account_name account, uint64_t id) {
          require_auth(account);

          ownerIndex owners(_self, _self);
          auto iterator = owners.find(id);
          eosio_assert(iterator == owners.end(), "owner already exists");
          owners.emplace(account, [&](auto& _owner) {
              _owner.account_name = account;
              _owner.id = id;
          });
        }

        // @abi action
        void Ticketing::addticket(const account_name account, uint64_t id, uint64_t owner, uint64_t category) {
                require_auth(account);

                ticketIndex tickets(_self, _self);
                auto ticket_iterator = tickets.find(id);
                eosio_assert(ticket_iterator == tickets.end(), "ticket already exists");
                tickets.emplace(account, [&](auto& _ticket) {
                    _ticket.account_name = account;
                    _ticket.id = id;
                    _ticket.owner_id = owner;
                    _ticket.category_id = category;
                    _ticket.burned = false;
                });

                ownerIndex owners(_self, _self);
                auto owner_iterator = owners.find(id);
                eosio_assert(owner_iterator == owners.end(), "ticket already exists");
                owners.modify(owner_iterator, account, [&](auto& owner) {
                    owner.tickets.push_back(id);
                });
        }

        // @abi action
        void Ticketing::burnticket(const account_name account, uint64_t id) {
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
        void Ticketing::getticket(const account_name account, uint64_t id) {
                require_auth(account);

                ticketIndex tickets(_self, _self);
                auto iterator = tickets.find(id);
                eosio_assert(iterator != tickets.end(), "ticket not found");
                auto ticket = (*iterator);

                print(" id: ", ticket.id,
                      " category: ", ticket.category_id,
                      " owner: ", ticket.owner_id,
                      " burned: ", ticket.burned
                      );
        }

        // @abi action
        void Ticketing::gettickets(const account_name account, uint64_t owner_id) {
                require_auth(account);

                ownerIndex owners(_self, _self);
                auto iterator = owners.find(owner_id);
                eosio_assert(iterator != owners.end(), "owner not found");
                auto owner = (*iterator);
                eosio_assert(owner.tickets.size() > 0, "no tickets for owner");
                uint64_t xa = 123;

                for (uint32_t i = 0; i < owner.tickets.size(); i++) {
                  print(owner.tickets.at(int (i)));
                }
        }

        // @abi action
        void Ticketing::swapowners(const account_name account, uint64_t t_id, uint64_t new_owner_id, uint64_t old_owner_id) {
          require_auth(account);

          ticketIndex tickets(_self, _self);
          auto iterator = tickets.find(t_id);
          eosio_assert(iterator != tickets.end(), "ticket not found");
          auto ticket = (*iterator);

          eosio_assert(ticket.burned != true, "ticket already burned");
          eosio_assert(ticket.account_name == account, "only creator can burn ticket");
          eosio_assert(ticket.owner_id == old_owner_id, "only valid current owner can be swapped");
          eosio_assert(new_owner_id != old_owner_id, "cannot swap to same owner");

          tickets.modify(iterator, account, [&](auto& tckt) {
              tckt.owner_id = new_owner_id;
          });
        }

        // @abi action
        void Ticketing::delticket(const account_name account, uint64_t id) {
          require_auth(account);

          ticketIndex tickets(_self, _self);
          auto iterator = tickets.find(id);
          eosio_assert(iterator != tickets.end(), "ticket not found");

          tickets.erase(iterator);
        }
}
