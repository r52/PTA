<template>
  <div>
    <h1 class="subtitle-1 text-center">Price Prediction via poeprices.info</h1>
    <p
      class="text-center my-1 price"
    >{{ state.prediction.min.toPrecision(4) }} ~ {{ state.prediction.max.toPrecision(4) }} {{ state.prediction.currency }}</p>
    <p class="text-center my-1">Confidence: {{ state.prediction.pred_confidence_score.toPrecision(4) }} %</p>
    <v-data-table :headers="headers" :items="listings" class="elevation-1"></v-data-table>
    <p class="text-center my-1">Please consider supporting poeprices.info</p>
  </div>
</template>

<script>
export default {
  name: "pred-tab",

  props: ["state"],

  computed: {
    listings() {
      let lst = [];

      this.state.prediction.pred_explanation.forEach(entry => {
        let obj = {};

        obj["name"] = entry[0];
        obj["contrib"] = (entry[1] * 100.0).toPrecision(4);

        lst.push(obj);
      });

      return lst;
    }
  },

  data() {
    return {
      headers: [
        {
          text: "Mod",
          align: "start",
          sortable: false,
          value: "name"
        },
        { text: "Contribution (%)", sortable: false, value: "contrib" }
      ]
    };
  }
};
</script>

<style scoped>
.price {
  color: #a38d6d;
  font-weight: bold;
}
</style>
