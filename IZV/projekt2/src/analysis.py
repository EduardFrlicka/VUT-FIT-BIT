#!/usr/bin/env python3.9
# coding=utf-8
from matplotlib import pyplot as plt
import pandas as pd
import seaborn as sns
import numpy as np
import os

# muzete pridat libovolnou zakladni knihovnu ci knihovnu predstavenou na prednaskach
# dalsi knihovny pak na dotaz

""" Ukol 1:
načíst soubor nehod, který byl vytvořen z vašich dat. Neznámé integerové hodnoty byly mapovány na -1.

Úkoly:
- vytvořte sloupec date, který bude ve formátu data (berte v potaz pouze datum, tj sloupec p2a)
- vhodné sloupce zmenšete pomocí kategorických datových typů. Měli byste se dostat po 0.5 GB. Neměňte však na kategorický typ region (špatně by se vám pracovalo s figure-level funkcemi)
- implementujte funkci, která vypíše kompletní (hlubkou) velikost všech sloupců v DataFrame v paměti:
orig_size=X MB
new_size=X MB

Poznámka: zobrazujte na 1 desetinné místo (.1f) a počítejte, že 1 MB = 1e6 B.
"""
REGIONS = ["KVK", "JHC", "JHM"]


def get_dataframe(filename: str, verbose: bool = False):
    """Funkcia nacita subor filename a upravi datove typy stlpcov pre nizsie naroky na pamet ako aj jednoduchiu pracu s datami.

    Args:
        filename (str): nazov suboru na citanie
        verbose (bool, optional: Povoluje vypis do konzole.
    """
    
    df = pd.read_pickle(filename)
    if verbose:
        orig = df.memory_usage(index=False, deep=True).sum() / 1000000

    for column_index in df:

        if df[column_index].dtype == "int64":
            df[column_index] = pd.to_numeric(df[column_index], downcast='signed')
            continue

        if df[column_index].dtype == "float64":
            df[column_index] = pd.to_numeric(df[column_index], downcast='float')
            continue

        if column_index == "region" or column_index == "p1":
            continue

        if column_index == "p2a":
            df["date"] = df[column_index].astype("datetime64")
            continue

        df[column_index] = df[column_index].astype("category")

    if verbose:
        new = df.memory_usage(index=False, deep=True).sum() / 1000000

        print("orig_size=%.1f MB" % orig)
        print("new_size=%.1f MB" % new)

    return df

# Ukol 2: počty nehod v jednotlivých regionech podle druhu silnic


def plot_roadtype(df: pd.DataFrame, fig_location: str = None,
                  show_figure: bool = False):

    plot_data = df[df["region"].isin(REGIONS)]
    plot_data = plot_data[["region", "p21", "p1"]]
    plot_data = plot_data.groupby(["region", "p21"]).agg("count").reset_index()

    # plot = sns.catplot(
    #     orient = "v",
    #     col = "p21",
    #     hue = "region",
    #     y = "p1",
    #     data=plot_data,
    #     kind="bar")

    # plt.show()

    pass

# Ukol3: zavinění zvěří


def plot_animals(df: pd.DataFrame, fig_location: str = None,
                 show_figure: bool = False):
    pass

# Ukol 4: Povětrnostní podmínky


def plot_conditions(df: pd.DataFrame, fig_location: str = None,
                    show_figure: bool = False):
    pass


if __name__ == "__main__":
    # zde je ukazka pouziti, tuto cast muzete modifikovat podle libosti
    # skript nebude pri testovani pousten primo, ale budou volany konkreni ¨
    # funkce.

    # tento soubor si stahnete sami, při testování pro hodnocení bude existovat
    df = get_dataframe("accidents.pkl.gz")
    # plot_roadtype(df, fig_location="01_roadtype.png", show_figure=True)
    # plot_animals(df, "02_animals.png", True)
    # plot_conditions(df, "03_conditions.png", True)
