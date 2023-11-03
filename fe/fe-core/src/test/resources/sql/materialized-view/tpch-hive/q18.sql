[sql]
select
    c_name,
    c_custkey,
    o_orderkey,
    o_orderdate,
    o_totalprice,
    sum(l_quantity)
from
    hive0.tpch.customer,
    hive0.tpch.orders,
    hive0.tpch.lineitem
where
        o_orderkey in (
        select
            l_orderkey
        from
            hive0.tpch.lineitem
        group by
            l_orderkey having
                sum(l_quantity) > 315
    )
  and c_custkey = o_custkey
  and o_orderkey = l_orderkey
group by
    c_name,
    c_custkey,
    o_orderkey,
    o_orderdate,
    o_totalprice
order by
    o_totalprice desc,
    o_orderdate limit 100;
[result]
TOP-N (order by [[12: o_totalprice DESC NULLS LAST, 13: o_orderdate ASC NULLS FIRST]])
    TOP-N (order by [[12: o_totalprice DESC NULLS LAST, 13: o_orderdate ASC NULLS FIRST]])
        AGGREGATE ([GLOBAL] aggregate [{52: sum=sum(52: sum)}] group by [[2: c_name, 1: c_custkey, 9: o_orderkey, 13: o_orderdate, 12: o_totalprice]] having [null]
            EXCHANGE SHUFFLE[2, 1, 9, 13, 12]
                AGGREGATE ([LOCAL] aggregate [{52: sum=sum(22: l_quantity)}] group by [[2: c_name, 1: c_custkey, 9: o_orderkey, 13: o_orderdate, 12: o_totalprice]] having [null]
                    LEFT SEMI JOIN (join-predicate [9: o_orderkey = 34: l_orderkey] post-join-predicate [null])
                        SCAN (mv[lineitem_mv] columns[99: c_name, 105: l_orderkey, 107: l_quantity, 114: o_custkey, 115: o_orderdate, 119: o_totalprice] predicate[null])
                        EXCHANGE BROADCAST
                            AGGREGATE ([GLOBAL] aggregate [{155: sum=sum(155: sum)}] group by [[56: l_orderkey]] having [155: sum > 315.00]
                                EXCHANGE SHUFFLE[56]
                                    AGGREGATE ([LOCAL] aggregate [{155: sum=sum(60: sum_qty)}] group by [[56: l_orderkey]] having [null]
                                        SCAN (mv[lineitem_agg_mv1] columns[56: l_orderkey, 60: sum_qty] predicate[null])
[end]

