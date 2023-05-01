#!/usr/bin/env python3
"""
IZV project part 1.

Author: Eduard Frlicka
Login:  xfrlic00
"""
# -*- coding: utf-8 -*-
import numpy as np
import matplotlib.pyplot as plt
import sys
import argparse

# povolene jsou pouze zakladni knihovny (os, sys) a knihovny numpy,
#   matplotlib a argparse

from download import DataDownloader

p24 = {
    0: "žádná místní úprava",
    1: "signalilzace na žlutou",
    2: "signalizace mimo provoz",
    3: "dopravními značkami",
    4: "přenosnými značkami",
    5: "přednost nevyznačena",
}


def plot_stat(data_source,
              fig_location=None,
              show_figure=False):
    """Generate 2 plots.

    Args:
        data_source (dict[str:np.array]): source data for plots.
        fig_location (str, optional): Set location for saving plot. If None,
            plot will not be saved. Defaults to None.
        show_figure (bool, optional): If True, shows plot. Defaults to False.
    """
    plt_data = []
    p24_values = p24.keys()

    regions_data = data_source["region"]
    p24_data = data_source["p24"]

    regions_sorted = sorted(DataDownloader().regions.keys())

    for region in regions_sorted:
        region_data = [0 for _ in p24_values]
        indices = np.where(regions_data == region)
        region_p24 = p24_data[indices]
        values, count = np.unique(region_p24, return_counts=True)
        for count_index, index in enumerate(values):
            region_data[index] = count[count_index]
        plt_data.append(region_data)
        # plt_data[region]=region_data
    plt_data = np.transpose(plt_data)
    plt_data = [*plt_data[1:], plt_data[0]]

    fig, (ax0, ax1) = plt.subplots(2, 1)
    # fig.tight_layout(rect=(0.25, 0, 1, 1))
    # fig.subplots_adjust(top=0.95, bottom=0.05, hspace=0.45)
    fig.set_size_inches(10, 6)

    np.seterr(divide='ignore')

    cm0 = ax0.pcolormesh(np.log10(plt_data), vmax=5)
    ax0.set_title("Absolutne")
    ax0.set_yticks([(x-1) % len(p24_values) + 0.5 for x in p24_values])
    ax0.set_yticklabels(list(p24.values()))
    ax0.set_xticks([x+0.5 for x in range(len(regions_sorted))])
    ax0.set_xticklabels(regions_sorted)
    ax0.invert_yaxis()

    plt_data_sums = [np.sum(x) for x in plt_data]

    ax1_data = [[cell / plt_data_sums[i] if cell else np.nan for cell in row]
                for i, row in enumerate(plt_data)]

    cm1 = ax1.pcolormesh(ax1_data, cmap="plasma", vmin=0, vmax=1)
    ax1.set_title("Relativne vuci pricine")
    ax1.set_yticks([(x-1) % len(p24_values) + 0.5 for x in p24_values])
    ax1.set_yticklabels(list(p24.values()))
    ax1.set_xticks([x+0.5 for x in range(len(regions_sorted))])
    ax1.set_xticklabels(regions_sorted)
    ax1.invert_yaxis()

    cbar0 = fig.colorbar(cm0, ax=ax0)
    cbar0.set_label("Pocet nehod 10^y")
    cbar1 = fig.colorbar(cm1, ax=ax1)
    cbar1.set_label("Podiel nehod pre danu pricinu")

    plt.tight_layout()
    # plt.autoscale()

    # pcm = ax1.imshow(relatives, cmap='plasma', vmax=100, vmin=0)
    # ax1.title.set_text('Relativně vůči přičině')

    # ax1.set_xticks(range(14))
    # ax1.set_xticklabels([sorted_regions[i] for i in range(14)])
    # ax1.set_yticks(range(6))
    # ax1.set_yticklabels([values_desc[i] for i in range(6)])

    # cbar = fig.colorbar(pcm, ax=ax1, shrink=1.1)
    # cbar.set_ticks([20*i for i in range(6)])
    # cbar.set_label('Počet nehod pro danou příčinu [%]', rotation=90)

    # ax0.figsize(8,8)

    if fig_location is not None:
        plt.savefig()

    if show_figure:
        plt.show()

# print(plt_data)

# for k,v in plt_data.items():
#     print(k,v)


#     for value in p24.keys():
#         # print(data_source[data_source==value])
#         # print(region)
#         pass
pass


def main():
    """Load arguments from console & call plot_stat with console arguments."""
    parser = argparse.ArgumentParser()
    parser.add_argument("--fig_location", required=False)
    parser.add_argument(
        "--show_figure", action='store_true', required=False)
    args = parser.parse_args()
    plot_stat(data_source=DataDownloader().get_dict(), **vars(args))
    pass


if __name__ == "__main__":
    main()
