# from pysol_cards.deal_game import Game

from pysol_cards.cards import CardRenderer
from pysol_cards.deal_game import Game
from pysol_cards.random import RandomBase


def pythonish_multiply(a, b):
    print("Will compute", a, "times", b)
    c = 0
    for i in range(0, a):
        c = c + b
    return c


rend = CardRenderer(True)


def create_gen(game_variant):
    ms = True
    which_deals = (RandomBase.DEALS_MS if ms else RandomBase.DEALS_PYSOLFC)
    game = Game(game_variant, 1, which_deals, 13)

    def _ret(deal_idx):
        s = game.calc_deal_string(deal_idx, rend)
        return s
    return _ret


def gen_pysol_deal(game_variant, deal_idx):
    ret = 0  # game.calc_deal_string(deal_idx, rend)
    return ret


class Gen:
    def __init__(self):
        self.game_variant = 'black_hole'
        # self.game = Game(self.game_variant, 1, self.which_deals, 13)
