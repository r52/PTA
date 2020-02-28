<template>
  <div>
    <p class="text-center my-1">Options: {{ state.results.options }}</p>
    <v-data-table :headers="headers" :items="listings" class="elevation-1 price"></v-data-table>
  </div>
</template>

<script>
export default {
  name: "result-tab",

  props: ["state"],

  computed: {
    type() {
      let type = "default";

      if (this.state.item.category == "gem") {
        type = "gem";
      }

      let format = this.state.results.format;

      if (format == "exchange") {
        type = "exchange";
      }

      return type;
    },
    headers() {
      let columns = {
        default: [
          {
            text: "Account Name",
            align: "start",
            sortable: false,
            value: "name"
          },
          {
            text: "Price",
            sortable: false,
            value: "price"
          },
          {
            text: "Age",
            sortable: false,
            value: "age"
          }
        ],
        gem: [
          {
            text: "Account Name",
            align: "start",
            sortable: false,
            value: "name"
          },
          {
            text: "Price",
            sortable: false,
            value: "price"
          },
          {
            text: "Q%",
            sortable: false,
            value: "quality"
          },
          {
            text: "Lvl",
            sortable: false,
            value: "level"
          },
          {
            text: "Age",
            sortable: false,
            value: "age"
          }
        ],
        exchange: [
          {
            text: "Account Name",
            align: "start",
            sortable: false,
            value: "name"
          },
          {
            text: "Price",
            sortable: false,
            value: "price"
          },
          {
            text: "Rate",
            sortable: false,
            value: "rate"
          },
          {
            text: "Age",
            sortable: false,
            value: "age"
          }
        ]
      };

      return columns[this.type];
    },
    listings() {
      let lst = [];
      let now = this.now();
      let type = this.type;

      this.state.results.result.forEach(entry => {
        let obj = {};

        // name
        obj["name"] = entry["listing"]["account"]["name"];

        // price
        if (type == "exchange") {
          let irate =
            entry["listing"]["price"]["item"]["amount"].toString() +
            " " +
            entry["listing"]["price"]["item"]["currency"];
          let erate =
            entry["listing"]["price"]["exchange"]["amount"].toString() +
            " " +
            entry["listing"]["price"]["exchange"]["currency"];

          obj["price"] = irate + " <= " + erate;
        } else {
          let amount = entry["listing"]["price"]["amount"];
          let currency = entry["listing"]["price"]["currency"];

          obj["price"] = amount.toString() + " " + currency;
        }

        // specials
        if (type == "exchange") {
          // rate
          let rate =
            entry["listing"]["price"]["item"]["amount"] /
            entry["listing"]["price"]["exchange"]["amount"];
          let ic = entry["listing"]["price"]["item"]["currency"];
          let ec = entry["listing"]["price"]["exchange"]["currency"];
          obj["rate"] = rate.toPrecision(3) + " " + ic + "/" + ec;
        }

        if (type == "gem") {
          // quality/level
          let q = "0%";
          let lvl = "1";
          let prop = entry["item"]["properties"];

          for (let p of prop) {
            if (p["name"] == "Quality") {
              let val = p["values"][0][0];
              q = val.replace(/^\D+/g, "");
            }

            if (p["name"] == "Level") {
              lvl = p["values"][0][0];
            }
          }

          obj["quality"] = q;
          obj["level"] = lvl;
        }

        // age
        obj["age"] = this.getRelTime(
          now,
          Date.parse(entry["listing"]["indexed"])
        );

        lst.push(obj);
      });

      return lst;
    }
  },

  methods: {
    now() {
      return Date.now();
    },
    getRelTime(t1, t2) {
      let dif = t1 - t2;

      // convert to seconds
      dif = Math.floor(dif / 1000);

      if (dif < 60) return dif.toString() + " seconds";

      // minutes
      if (dif < 3600) return Math.floor(dif / 60).toString() + " minutes";

      if (dif < 86400) return Math.floor(dif / 3600).toString() + " hours";

      return Math.floor(dif / 86400).toString() + " days";
    }
  }
};
</script>

<style scoped>
.price {
  color: #a38d6d;
}

.v-data-table /deep/ td {
  font-size: 18px !important;
}
</style>
