# from pysol_cards.deal_game import Game


def pythonish_multiply(a, b):
    print("Will compute", a, "times", b)
    c = 0
    for i in range(0, a):
        c = c + b
    return c


class Gen:
    def __init__(self):
        self.game_variant = 'black_hole'
        # self.game = Game(self.game_variant, 1, self.which_deals, 13)
