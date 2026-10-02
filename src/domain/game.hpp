#pragma once

#include "dealer.hpp"
#include "deck.hpp"
#include "event.hpp"
#include "player.hpp"
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
        Game(int decks, int starting_chips)
            : player_{ starting_chips }
            , dealer_{}
            , deck_{ decks, true } {
        }

        Game(Player player, Dealer dealer, Deck deck)
            : player_(std::move(player))
            , dealer_(std::move(dealer))
            , deck_(std::move(deck)) {
        }

        ~Game() = default;

        Game(const Game&) = delete;
        Game& operator=(const Game&) = delete;

        Game(Game&&) noexcept = default;
        Game& operator=(Game&&) noexcept = default;

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

        // TODO: add tests for these 5 methods
        [[nodiscard]] bool can_place_bet(int amount) const noexcept {
            return state_ == GameState::WaitingForBets && amount > 0 && amount <= player_.chips();
        }

        [[nodiscard]] bool can_hit() const noexcept {
            return state_ == GameState::PlayerTurn;
        }

        [[nodiscard]] bool can_stand() const noexcept {
            return state_ == GameState::PlayerTurn;
        }

        [[nodiscard]] bool can_split() const noexcept {
            return state_ == GameState::PlayerTurn && player_.can_split() && player_.chips() >= total_bet();
        }

        [[nodiscard]] bool can_double_down() const noexcept {
            return state_ == GameState::PlayerTurn && player_.active_hand().hand.size() == 2 && player_.chips() >= total_bet();
        }

        void deal_initial_cards();
        void deal_dealer_cards();

        void hit();
        void stand();
        void double_down();
        void split();
        void evaluate_hand(size_t hand_index);
        RoundSummary finish_round();

        void start_round(int bet) {
            if (!can_place_bet(bet)) {
                throw std::runtime_error("Cannot start round: invalid bet amount or game already in progress.");
            }
            discard_cards();
            place_bet(bet);
            set_state(domain::GameState::Dealing);

            if (deck_.needs_reshuffle()) {
                deck_.reshuffle_discarded_into_deck();
            }

            deal_initial_cards();
        }

        void discard_cards() {
            deck_.discard_cards(player_.discard_hand());
            deck_.discard_cards(dealer_.discard_hand());
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

        [[nodiscard]] int payout();

        std::vector<GameEvent> events_;
    };
} // namespace domain