<template>
  <v-form>
    <v-container class="font-weight-light">
      <v-row align="end" align-content="end" justify="space-between" no-gutters>
        <v-col cols="6">
          <p class="mb-1">Mods</p>
        </v-col>
        <v-col cols="1">
          <p class="mb-1">Min</p>
        </v-col>
        <v-col cols="1">
          <p class="text-center mb-1">Cur</p>
        </v-col>
        <v-col cols="1">
          <p class="mb-1">Max</p>
        </v-col>
      </v-row>
      <v-divider />
      <!-- weapon/armour -->
      <base-mod-filter
        v-if="state.item.weapon && state.item.weapon.pdps"
        label="Physical DPS"
        :searchopts="state.searchopts"
        :settings="state.settings"
        type="use_pdps"
        :current="getAvg(state.item.weapon.pdps) * state.item.weapon.aps"
      />
      <base-mod-filter
        v-if="state.item.weapon && state.item.weapon.edps"
        label="Elemental DPS"
        :searchopts="state.searchopts"
        :settings="state.settings"
        type="use_edps"
        :current="getAvg(state.item.weapon.edps) * state.item.weapon.aps"
      />
      <base-mod-filter
        v-if="state.item.armour && state.item.armour.ar"
        label="Armour"
        :searchopts="state.searchopts"
        :settings="state.settings"
        type="use_ar"
        :current="state.item.armour.ar"
      />
      <base-mod-filter
        v-if="state.item.armour && state.item.armour.ev"
        label="Evasion"
        :searchopts="state.searchopts"
        :settings="state.settings"
        type="use_ev"
        :current="state.item.armour.ev"
      />
      <base-mod-filter
        v-if="state.item.armour && state.item.armour.es"
        label="Energy Shield"
        :searchopts="state.searchopts"
        :settings="state.settings"
        type="use_es"
        :current="state.item.armour.es"
      />
      <v-divider v-if="state.item.weapon || state.item.armour" />
      <!-- mods -->
      <mod-filter
        v-for="filter in state.item.filters"
        :key="filter.id"
        :filter="filter"
        :settings="state.settings"
        type="normal"
      />
      <v-divider />
      <mod-filter
        v-for="filter in state.item.pseudos"
        :key="filter.id"
        :filter="filter"
        :settings="state.settings"
        type="pseudo"
      />
      <v-divider v-if="state.item.pseudos" />
      <!-- misc option checkboxes -->
      <v-row align="center" align-content="center" justify="space-between" no-gutters>
        <div class="d-inline-flex">
          <!-- TODO corrupted -->
          <mod-checkbox
            v-if="state.item.sockets"
            v-model="state.searchopts.use_sockets"
            :label="`Use Sockets (${state.item.sockets.total})`"
          />
          <mod-checkbox
            v-if="state.item.sockets"
            v-model="state.searchopts.use_links"
            :label="`Use Links (${state.item.sockets.links})`"
          />
          <mod-checkbox label="iLvl (min):" v-model="state.searchopts.use_ilvl" />
          <v-text-field
            single-line
            dense
            hide-details
            type="number"
            v-model.number="state.item.ilvl"
            @input="state.searchopts.use_ilvl = true"
            @keypress="isNumber($event)"
            class="my-0 py-0"
          />
          <mod-checkbox label="Use Item Base" v-model="state.searchopts.use_item_base" />
          <div class="d-inline-flex" v-if="state.item.influences && state.item.influences.length">
            <mod-checkbox
              v-for="inf in state.item.influences"
              v-model="state.searchopts.influences"
              multiple
              :key="inf"
              :label="capitalize(inf)"
              :value="inf"
              color="purple"
            />
          </div>
          <mod-checkbox
            v-if="state.item.misc && state.item.misc.synthesis"
            label="Synthesis"
            v-model="state.searchopts.use_synthesis_base"
            color="orange"
          />
        </div>
      </v-row>
      <v-divider />
      <!-- buttons -->
      <v-row align="end" align-content="end" justify="end">
        <v-btn class="mx-1 mt-1" color="purple" @click="search(true)">Open on pathofexile.com</v-btn>
        <v-btn class="mx-1 mt-1" color="primary" @click="search(false)">Search</v-btn>
      </v-row>
    </v-container>
  </v-form>
</template>

<script>
import ModFilter from "./ModFilter.vue";
import BaseModFilter from "./BaseModFilter.vue";
import ModCheckbox from "./ModCheckbox.vue";

export default {
  name: "mod-tab",

  components: {
    ModFilter,
    BaseModFilter,
    ModCheckbox
  },

  props: ["state"],

  methods: {
    capitalize: s => {
      s = s.charAt(0).toUpperCase() + s.slice(1);
      return s;
    },
    getAvg: prop => {
      return (prop.min + prop.max) / 2;
    },
    isNumber: function(evt) {
      evt = evt ? evt : window.event;
      var charCode = evt.which ? evt.which : evt.keyCode;
      if (
        charCode > 31 &&
        (charCode < 48 || charCode > 57) &&
        charCode !== 46 &&
        charCode !== 45
      ) {
        evt.preventDefault();
      } else {
        return true;
      }
    },
    search(openbrowser) {
      this.$api.then(pta => {
        pta.advancedPriceCheck(JSON.stringify(this.state), openbrowser);
      });
    }
  }
};
</script>
