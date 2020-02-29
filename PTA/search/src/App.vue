<template>
  <v-app>
    <v-content v-if="state.item">
      <v-card-title class="text-center justify-center py-2 d-block">
        <h1 class="title" :class="[itemClass]">
          {{ state.item.name }}
          {{ state.item.type }}
        </h1>
        <h1 class="subtitle-1 unidentifiedItem" v-if="state.item.unidentified">Unidentified</h1>
        <h1 class="subtitle-1 corruptedItem" v-if="state.item.corrupted">Corrupted</h1>
        <h1
          class="subtitle-1 influenceItem"
          v-if="state.item.influences"
        >{{ capitalizeList(state.item.influences) }}</h1>
        <h1 class="subtitle-1 green--text">{{ state.settings.league }}</h1>
      </v-card-title>

      <v-tabs v-model="state.tab" background-color="transparent" grow>
        <v-tab v-if="state.results" href="#results">Results</v-tab>
        <v-tab v-if="state.prediction" href="#prediction">poeprices.info</v-tab>
        <v-tab v-if="state.item.filters" href="#mods">Advanced</v-tab>
      </v-tabs>

      <v-tabs-items v-model="state.tab">
        <v-tab-item v-if="state.results" value="results">
          <result-tab :state="state" />
        </v-tab-item>
        <v-tab-item v-if="state.prediction" value="prediction">
          <pred-tab :state="state" />
        </v-tab-item>
        <v-tab-item v-if="state.item.filters" value="mods">
          <mod-tab :state="state" />
        </v-tab-item>
      </v-tabs-items>
      <v-card-actions>
        <v-spacer></v-spacer>
        <v-btn color="error" @click="close()" accesskey="c"><u>C</u>lose</v-btn>
      </v-card-actions>
    </v-content>
  </v-app>
</template>

<script>
import ModTab from "./components/ModTab.vue";
import PredTab from "./components/PredTab.vue";
import ResultTab from "./components/ResultTab.vue";

let dtm = {};

if (process.env.NODE_ENV === "development") {
  let sample = require("./test/sampledata.js");
  dtm = sample.item;
} else {
  // eslint-disable-next-line
  dtm = item;
}

let init = {
  tab: null,
  prediction: null,
  results: null
};

export default {
  name: "App",

  components: {
    ModTab,
    PredTab,
    ResultTab
  },

  data() {
    const state = Object.assign(init, dtm);
    return { state: state };
  },

  computed: {
    itemClass() {
      let cls = this.state.item.rarity;

      if (
        this.state.item.category == "gem" ||
        this.state.item.category == "prophecy" ||
        this.state.item.category == "card" ||
        this.state.item.category == "currency"
      ) {
        cls = this.state.item.category;
      }

      return cls;
    }
  },

  methods: {
    close: () => {
      window.close();
    },
    capitalizeList: l => {
      const capl = [...l];
      capl.forEach((o, i, a) => {
        a[i] = o.charAt(0).toUpperCase() + o.slice(1);
      });

      return capl.join(", ");
    },
    processPriceResults(res) {
      let jres = JSON.parse(res);
      this.state.results = jres;
      this.state.tab = "results";
    }
  },

  created() {
    this.$vuetify.theme.dark = true;

    this.$api.then(pta => {
      pta.priceCheckFinished.connect(this.processPriceResults);
    });
  }
};
</script>

<style>
@font-face {
  font-family: "Fontin-SmallCaps";
  src: url(./assets/Fontin-SmallCaps.ttf) format("truetype");
}

html {
  overflow-y: auto !important;
}

.corruptedItem,
.unidentifiedItem {
  color: #d20000;
}

.influenceItem {
  color: #8e44ad;
}

.Unique {
  border-color: #af6025;
  color: #af6025;
}

.Magic {
  border-color: #88f;
  color: #88f;
}

.Normal {
  border-color: #c8c8c8;
  color: #c8c8c8;
}

.Rare {
  border-color: rgb(255, 255, 119);
  color: rgb(255, 255, 119);
}

.prophecy {
  border-color: #b54bff;
  color: #b54bff;
}

.gem {
  border-color: #1ba29b;
  color: #1ba29b;
}

.card {
  border-color: #111;
  color: #eee;
}
</style>
