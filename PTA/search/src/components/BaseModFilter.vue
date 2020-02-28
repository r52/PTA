<template>
  <v-row align="center" align-content="center" justify="space-between" no-gutters>
    <v-col cols="6">
      <v-switch
        dense
        hide-details
        class="my-1 py-0 magic--text"
        v-model="searchopts[type].enabled"
        :label="label"
      />
    </v-col>
    <v-col cols="1">
      <mod-num-input type="min" :filter="searchopts[type]" :settings="settings" />
    </v-col>
    <v-col cols="1" align-self="center">
      <p class="text-center my-0 py-0">{{ current }}</p>
    </v-col>
    <v-col cols="1">
      <mod-num-input type="max" :filter="searchopts[type]" :settings="settings" />
    </v-col>
  </v-row>
</template>

<script>
import ModNumInput from "./ModNumInput.vue";

export default {
  name: "base-mod-filter",

  components: {
    ModNumInput
  },

  props: {
    label: String,
    searchopts: Object,
    settings: Object,
    type: String,
    current: Number
  },

  created() {
    let range = this.settings.prefillrange / 100.0;
    let value = this.current;
    let diff = range * value;

    if (this.settings.prefillmin) {
      this.searchopts[this.type]["min"] = value - diff;
    }

    if (this.settings.prefillmax) {
      this.searchopts[this.type]["max"] = value + diff;
    }
  }
};
</script>

<style scoped>
.magic--text /deep/ label {
  color: #88f;
}
</style>
