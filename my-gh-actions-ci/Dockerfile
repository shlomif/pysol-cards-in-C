FROM fedora:42

RUN echo helloworld

RUN expr 4 '*' 6

COPY . /git
RUN dnf -y install clang cmake diffutils freecell-solver make perl-core perl-devel python3-devel python3-pip python3-pysol-cards

RUN pip install --upgrade pysol_cards

RUN set -e -x; cd /git ; gmake retest

