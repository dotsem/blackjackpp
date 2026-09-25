#pragma once

#include "domain/dealer.hpp"
#include "domain/deck.hpp"
#include "domain/game.hpp"

namespace test_helpers {

inline domain::Game create_game(int chips = 1000) {
    domain::Player player(chips);
    domain::Dealer dealer;
    domain::Deck deck;
    domain::Game game(player, dealer, deck);
    return game;
}

inline domain::Game create_split_game(int chips = 1000) {
    domain::Player player(chips);
    domain::Card card1{domain::Suit::Hearts, domain::Rank::Eight};
    domain::Card card2{domain::Suit::Diamonds, domain::Rank::Eight};
    player.add_card(card1);
    player.add_card(card2);
    player.active_hand().bet = 100;
    player.split();

    domain::Dealer dealer;
    domain::Deck deck;
    domain::Game game(player, dealer, deck);
    return game;
}
} // namespace test_helpers