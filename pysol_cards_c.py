from pysol_cards.cards import CardRenderer
from pysol_cards.deal_game import Game
from pysol_cards.random import RandomBase


rend = CardRenderer(True)


def create_gen(game_variant, ms):
    which_deals = (
        RandomBase.DEALS_MS if (ms == 0) else RandomBase.DEALS_PYSOLFC
    )
    game = Game(game_variant, 1, which_deals, 13)

    def _ret(deal_idx):
        s = game.calc_deal_string(deal_idx, rend)
        return s
    return _ret
