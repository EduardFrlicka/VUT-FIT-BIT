#!/usr/bin/env python3
"""
IZV project part 1.

Author: Eduard Frlicka
Login:  xfrlic00
"""
# -*- coding: utf-8 -*-

import os
import sys
import re
import gzip
import pickle
import csv
import zipfile
import numpy as np
from bs4 import BeautifulSoup
import requests
import io

# Kromě vestavěných knihoven (os, sys, re, requests …) byste si měli vystačit
# s: gzip, pickle, csv, zipfile, numpy, matplotlib, BeautifulSoup.
# Další knihovny je možné použít po schválení opravujícím (např ve fóru WIS).


# def timing(f):
#     @wraps(f)
#     def wrap(*args, **kw):
#         ts = time.time()
#         result = f(*args, **kw)
#         te = time.time()
#         print('func:%r args:[%r, %r] took: %2.4f sec' %
#               (f.__name__, args, kw, te-ts)
#             #   , file=sys.stderr
#               )
#         return result
#     return wrap


class DataDownloader:
    """
        TODO: dokumentacni retezce.

    Attributes:
        headers    Nazvy hlavicek jednotlivych CSV souboru, tyto nazvy nemente!
        regions     Dictionary s nazvy kraju : nazev csv souboru
    """

    headers = ["p1", "p36", "p37", "p2a", "weekday(p2a)", "p2b", "p6", "p7",
               "p8", "p9", "p10", "p11", "p12", "p13a", "p13b", "p13c", "p14",
               "p15", "p16", "p17", "p18", "p19", "p20", "p21", "p22", "p23",
               "p24", "p27", "p28", "p34", "p35", "p39", "p44", "p45a", "p47",
               "p48a", "p49", "p50a", "p50b", "p51", "p52", "p53", "p55a",
               "p57", "p58", "a", "b", "d", "e", "f", "g", "h", "i", "j",
               "k", "l", "n", "o", "p", "q", "r", "s", "t", "p5a"]

    regions = {
        "PHA": "00",
        "STC": "01",
        "JHC": "02",
        "PLK": "03",
        "ULK": "04",
        "HKK": "05",
        "JHM": "06",
        "MSK": "07",
        "OLK": "14",
        "ZLK": "15",
        "VYS": "16",
        "PAK": "17",
        "LBK": "18",
        "KVK": "19",
    }

    region_data = {}

    @staticmethod
    def conv_float(x: str):
        """
            Function converts string from parameter x to np.float64.

        Args:
            x (str): string representation of float number.

        Returns:
            np.float64: float representation of string x.
        """
        try:
            return np.float64(
                x.replace(",", "."))
        except ValueError:
            return np.nan

    @staticmethod
    def conv_string(x: str):
        """
            Just placeholder function.

        Args:
            x (str):

        Returns:
            str: returns argument x
        """
        return x

    @staticmethod
    def conv_int(x: str):
        """
            Function converts string from parameter x to np.float64.

        Args:
            x (str): string representation of integer number.

        Returns:
            int: integer representation of x.
        """
        return int(x) if x != "" else -1

    converters = {
        0: conv_string.__func__,
        3: lambda x:  np.datetime64(x),
        5: conv_string.__func__,
        34: lambda x: int(x) if x != "XX" and x != "" else -1,
        45: conv_float.__func__,
        46: conv_float.__func__,
        47: conv_float.__func__,
        48: conv_float.__func__,
        49: conv_float.__func__,
        50: conv_float.__func__,
        51: conv_string.__func__,
        52: conv_string.__func__,
        53: conv_string.__func__,
        54: conv_string.__func__,
        55: conv_string.__func__,
        56: conv_string.__func__,
        57: conv_string.__func__,
        58: conv_string.__func__,
        59: conv_string.__func__,
        60: conv_string.__func__,
        61: conv_string.__func__,
        62: conv_string.__func__,
        63: conv_string.__func__,
    }

    def __init__(self, url="https://ehw.fit.vutbr.cz/izv/",
                 folder="data", cache_filename="data_{}.pkl.gz"):
        """
            Initialize DataDownloader class.

        Args:
            url (str, optional): Defines url address from which will be
                scraped and downloaded data.
                Defaults to "https://ehw.fit.vutbr.cz/izv/".
            folder (str, optional): Defines name of folder where data
                will be stored. Defaults to "data".
            cache_filename (str, optional): Defines format for cache
                file name. Defaults to "data_{}.pkl.gz".
        """
        self.url = url
        self.folder = folder
        self.cache_filename = cache_filename
        self._zipfile_addr = None
        self._zipfile_names = None
        pass

    def download_file(self, path: str):
        """Download file from urlpath.

        Args:
            path (str): path to file on self.url servet

        Raises:
            Exception: if web request status code differs from OK (200).
        """
        # print("Downloading file:", path)
        basename = os.path.basename(path)
        os.makedirs(self.folder, exist_ok=True)
        page = requests.get(f"{self.url}/{path}")
        if(page.status_code != 200):
            raise Exception(f"Request status code: {page.status_code}")
        with open(os.path.join(self.folder, basename), "wb") as file:
            file.write(page.content)

    @property
    def zipfile_addr(self):
        """
            Return list of file paths on self.url server.

            After scraping stores paths in attriute self._zipfile_addr
            On repeated calls just returns attriute self._zipfile_addr

        Raises:
            Exception: if request status code is not OK (200)

        Returns:
            list[str]: list of paths
        """
        if self._zipfile_addr is not None:
            return self._zipfile_addr

        self._zipfile_addr = []
        page = requests.get(self.url)
        if(page.status_code != 200):
            raise Exception(f"Request status code: {page.status_code}")

        soup = BeautifulSoup(page.content, "lxml")
        rows = soup.find_all("tr")
        for row in rows:
            onclick = row.find_all("button")[-1].get("onclick")
            match = re.match(r"download\('(.*)'\)", onclick.__str__())
            if match is None:
                continue
            self._zipfile_addr.append(match.group(1))

        return self._zipfile_addr

    @property
    def zipfile_names(self):
        """
            Convert zipfile_addr to list of basenames.

        Returns:
            list[str]: list of basenames

        """
        if self._zipfile_names is not None:
            return self._zipfile_names

        self._zipfile_names = []
        for i in self.zipfile_addr:
            self._zipfile_names.append(os.path.basename(i))
        return self._zipfile_names

    def download_data(self):
        """
        download all files from zipfile_addr and stores them
            in self.folder folder.
            """
        for file in self.zipfile_addr:
            self.download_file(file)

    def get_zipfiles(self):
        """
            Ensure that all zipfiles needed are downloaded.

            Download missing ones.

        Returns:
            list[str]: returns basenames of zipfiles
        """
        if not os.path.isdir(self.folder):
            self.download_data()
        folder_files = os.listdir(self.folder)
        i = 0
        while i < len(self.zipfile_names):
            file = self.zipfile_names[i]
            # print("Checking file:", file)
            if file not in folder_files:
                self.download_file(self.zipfile_addr[i])
                folder_files = os.listdir(self.folder)
                continue
            i += 1
        return self.zipfile_names

    def parse_region_data(self, region):
        """
            open, reads and parses csv files from each zipfile,
                for selected region.

        Args:
            region (str): three letter code representing region

        Raises:
            AttributeError: when arg region is not in attribute regions
            ValueError: if csv file has wrong number of columns

        Returns:
            dict[str:np.array]: dictionary (header : numpy array of values)
        """
        if region not in self.regions:
            raise AttributeError(f"{region} is not valid region argument")

        header_count = len(self.headers)
        res = [[] for i in self.headers]
        added = set()
        # control = {}

        for zipfile_name in self.get_zipfiles():
            # print("Parsing:", region, "from file:", zipfile_name)
            csv_name = f"{self.regions[region]}.csv"
            zipfile_path = os.path.join(self.folder, zipfile_name)
            with zipfile.ZipFile(zipfile_path).open(csv_name, "r") as file:
                reader = csv.reader(io.TextIOWrapper(
                    file, encoding="cp1250"), delimiter=";")
                for line in reader:
                    if len(line) != header_count:
                        raise ValueError("line has %d columns, %d expected" % (
                            len(line), header_count))

                    id = line[0]
                    if id in added:
                        # print("duplicate", id, zipfile_name)
                        continue
                    added.add(id)
                    for i, col in enumerate(line):
                        res[i].append(self.converters.get(
                            i, self.conv_int)(col))
        res_dict = {}

        for i, column in enumerate(res):
            res_dict[self.headers[i]] = np.array(column)
        res_dict["region"] = np.full(len(res[0]), region)

        # print("pocet zaznamov:", len(res[0]))
        return res_dict

    def get_region_data(self, region: str):
        """
            Parse, cache and store region data.

            When called repeatedly, uses stored data from attribute.
            If cahce file avaliable loads data from cache.

        Args:
            region (str): three letter code representing region

        Returns:
            dict[str:np.array]: dictionary (header : numpy array of values)
        """
        region_data = self.region_data.get(region)
        if region_data is not None:
            # print("Loading from memory:", region)
            return region_data

        cache_name = os.path.join(
            self.folder, self.cache_filename.format(region))
        if os.path.isfile(cache_name):
            # print("Loading from cache:", region)
            with gzip.open(cache_name, "rb") as file:
                region_data = pickle.load(file)
                self.region_data[region] = region_data
                return region_data

        # print("Parsing:", region)
        region_data = self.parse_region_data(region)
        self.region_data[region] = region_data

        with gzip.open(cache_name, "wb", 7) as file:
            pickle.dump(region_data, file)
        return region_data

    def get_dict(self, regions: list[str] = None):
        """
            Concatenate dictionaries from function get_region_data.

        Args:
            regions (list[str]): list of three letter codes
                representing regions

        Returns:
            dict[str:np.array]: dictionary (header : numpy array of values)
        """
        if regions is None or regions == []:
            regions = list(self.regions.keys())
        if type(regions) == str:
            regions = [regions]

        if type(regions) != list:
            raise AttributeError(
                f"""Attribute regions in function get_dict
                    has not valid value: {regions}""")

        res = {i: [] for i in [*self.headers, "region"]}

        for region in regions:
            region_data = self.get_region_data(region)
            for key, value in region_data.items():
                res[key].append(value)
            # print(len(region_data["region"]))
            # print("\n")
            pass

        res_dict = {}
        for key, value in res.items():
            res_dict[key] = np.concatenate(value)

        return res_dict


def main():
    """
    main function.

        calls get_dict() and prints basic data about dictionary.
    """
    np.set_printoptions(linewidth=1000)
    c = DataDownloader()
    regions = ["PHA", "KVK", "HKK"]
    dict = c.get_dict(regions)
    print("%12s: %15s: %5s: %s:" % ("header", "type", "length", "data"))
    for key, value in dict.items():
        print("%12s: %15s: %5d: %s" % (key, value.dtype, len(value),  value))
    print("\nRegions in dataset:", " ".join(regions))


if __name__ == "__main__":
    main()
