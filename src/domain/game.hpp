#pragma once

#include "domain/dealer.hpp"
#include "domain/deck.hpp"
#include "domain/event.hpp"
#include "domain/player.hpp"
#include <optional>

namespace domain {

    enum class GameState : std::uint8_t {
        WaitingForBets,
        Dealing,
        PlayerTurn,
        DealerTurn,
        RoundOver
    };

    enum class GameResult : std::uint8_t {
        PlayerBust,
        DealerBust,
        PlayerBlackjack,
        DealerBlackjack,
        Push
    };

    struct RoundSummary {
        int total_bet{ 0 };
        int total_payout{ 0 };

        [[nodiscard]] int net_profit() const noexcept {
            return total_payout - total_bet;
        }

        [[nodiscard]] bool is_profit() const noexcept {
            return net_profit() > 0;
        }
    };

    class Game {
    public:
        Game(Player player, Dealer dealer, Deck deck)
            : player_(std::move(player))
            , dealer_(std::move(dealer))
            , deck_(std::move(deck)) {
        }

        [[nodiscard]] int total_bet() const noexcept {
            int total = 0;
            for (const auto& hand : player_.hands()) {
                total += hand.bet;
            }
            return total;
        }

        [[nodiscard]] GameState state() const noexcept {
            return state_;
        }

        void set_state(GameState new_state) {
            state_ = new_state;
        }

        [[nodiscard]] const std::optional<GameResult>& result() const noexcept {
            return result_;
        }

        [[nodiscard]] const Player& player() const noexcept {
            return player_;
        }

        [[nodiscard]] const Dealer& dealer() const noexcept {
            return dealer_;
        }

        [[nodiscard]] const Deck& deck() const noexcept {
            return deck_;
        }

        void deal_initial_cards();
        void deal_dealer_cards();

        void hit();
        void stand();
        void double_down();
        void split();
        void evaluate_hand(int hand_index);
        RoundSummary finish_round();

        void place_bet(int amount) {
            if (state_ != GameState::WaitingForBets) {
                throw std::runtime_error("Cannot place bet at this time.");
            }
            if (amount <= 0 || amount > player_.chips()) {
                throw std::runtime_error("Invalid bet amount.");
            }
            player_.active_hand().bet = amount;
            player_.remove_chips(amount);
        }

        void reset_game() {
            player_.clear_hands();
            dealer_.clear_hand();
            deck_.reset();
            state_ = GameState::WaitingForBets;
            result_ = std::nullopt;
        }

        [[nodiscard]] std::vector<GameEvent> poll_events() {
            std::vector<GameEvent> drained = std::move(events_);
            events_.clear();
            return drained;
        }

    private:
        Player player_;
        Dealer dealer_;
        Deck deck_;
        GameState state_{ GameState::WaitingForBets };
        std::optional<GameResult> result_{ std::nullopt };

        void deal_card_to_dealer(bool is_face_up = true) {
            auto card_opt = deck_.draw_card();
            if (!card_opt) {
                throw std::runtime_error("Deck is empty. Cannot draw a card.");
            }
            if (is_face_up) {
                dealer_.add_upcard(*card_opt);
            } else {
                dealer_.add_hole_card(*card_opt);
            }
            events_.emplace_back(CardDealtEvent{ .card = *card_opt, .is_dealer = true, .hand_index = 0, .is_face_up = is_face_up });
        }

        void deal_card_to_player_hand(size_t hand_index) {
            auto card_opt = deck_.draw_card();
            if (!card_opt) {
                throw std::runtime_error("Deck is empty. Cannot draw a card.");
            }
            player_.add_card_to_hand(*card_opt, hand_index);
            events_.emplace_back(CardDealtEvent{ .card = *card_opt, .is_dealer = false, .hand_index = hand_index, .is_face_up = true });
        }

        void deal_card_to_player_current_hand() {
            deal_card_to_player_hand(player_.active_hand_index());
        }

        [[nodiscard]] int payout();

        std::vector<GameEvent> events_;
    };
} // namespace domain