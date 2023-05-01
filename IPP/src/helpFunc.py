from re import findall, search, Match
import xml.etree.ElementTree as et
import sys
import traceback


def error_exit(value, message):
    traceback.print_stack()
    print(message, flush=True, file=sys.stderr)
    sys.exit(value)


def getOrder(elem: et.Element):
    order = elem.attrib.get("order")
    if order == None:
        error_exit(
            32, f"getOrder: Bad XML, no order attribute in instruction element")

    if order.startswith("-"):
        error_exit(32, "getOrder: Bad XML, order < 0")

    if not order.isnumeric():
        error_exit(32, f"getOrder: Bad XML, order ({order}) is not a number")

    order = int(order)
    if order == 0:
        error_exit(32, "getOrder: Bad XML, order ==0")

    return order
